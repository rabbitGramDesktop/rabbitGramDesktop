/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#include "boxes/send_credits_box.h"

#include "api/api_credits.h"
#include "apiwrap.h"
#include "core/ui_integration.h" // Core::MarkedTextContext.
#include "data/data_credits.h"
#include "data/data_photo.h"
#include "data/data_session.h"
#include "data/data_user.h"
#include "data/stickers/data_custom_emoji.h"
#include "history/history.h"
#include "history/history_item.h"
#include "info/channel_statistics/boosts/giveaway/boost_badge.h" // InfiniteRadialAnimationWidget.
#include "lang/lang_keys.h"
#include "main/main_session.h"
#include "payments/payments_checkout_process.h"
#include "payments/payments_form.h"
#include "settings/settings_credits_graphics.h"
#include "ui/controls/userpic_button.h"
#include "ui/effects/premium_graphics.h"
#include "ui/effects/premium_top_bar.h" // Ui::Premium::ColorizedSvg.
#include "ui/image/image_prepare.h"
#include "ui/layers/generic_box.h"
#include "ui/rect.h"
#include "ui/text/text_utilities.h"
#include "ui/vertical_list.h"
#include "ui/widgets/buttons.h"
#include "styles/style_boxes.h"
#include "styles/style_credits.h"
#include "styles/style_giveaway.h"
#include "styles/style_layers.h"
#include "styles/style_premium.h"
#include "styles/style_settings.h"

namespace Ui {
namespace {

struct PaidMediaData {
	const Data::Invoice *invoice = nullptr;
	HistoryItem *item = nullptr;
	PeerData *peer = nullptr;
	int photos = 0;
	int videos = 0;

	explicit operator bool() const {
		return invoice && item && peer && (photos || videos);
	}
};

[[nodiscard]] PaidMediaData LookupPaidMediaData(
		not_null<Main::Session*> session,
		not_null<Payments::CreditsFormData*> form) {
	using namespace Payments;
	const auto message = std::get_if<InvoiceMessage>(&form->id.value);
	const auto item = message
		? session->data().message(message->peer, message->itemId)
		: nullptr;
	const auto media = item ? item->media() : nullptr;
	const auto invoice = media ? media->invoice() : nullptr;
	if (!invoice || !invoice->isPaidMedia) {
		return {};
	}

	auto photos = 0;
	auto videos = 0;
	for (const auto &media : invoice->extendedMedia) {
		const auto photo = media->photo();
		if (photo && !photo->extendedMediaVideoDuration().has_value()) {
			++photos;
		} else {
			++videos;
		}
	}

	const auto sender = item->originalSender();
	const auto broadcast = (sender && sender->isBroadcast())
		? sender
		: message->peer.get();
	return {
		.invoice = invoice,
		.item = item,
		.peer = broadcast,
		.photos = photos,
		.videos = videos,
	};
}

[[nodiscard]] rpl::producer<TextWithEntities> SendCreditsConfirmText(
		not_null<Main::Session*> session,
		not_null<Payments::CreditsFormData*> form) {
	if (const auto data = LookupPaidMediaData(session, form)) {
		auto photos = 0;
		auto videos = 0;
		for (const auto &media : data.invoice->extendedMedia) {
			const auto photo = media->photo();
			if (photo && !photo->extendedMediaVideoDuration().has_value()) {
				++photos;
			} else {
				++videos;
			}
		}

		auto photosBold = tr::lng_credits_box_out_photos(
			lt_count,
			rpl::single(photos) | tr::to_count(),
			Ui::Text::Bold);
		auto videosBold = tr::lng_credits_box_out_videos(
			lt_count,
			rpl::single(videos) | tr::to_count(),
			Ui::Text::Bold);
		auto media = (!videos)
				? ((photos > 1)
					? std::move(photosBold)
					: tr::lng_credits_box_out_photo(Ui::Text::WithEntities))
				: (!photos)
				? ((videos > 1)
					? std::move(videosBold)
					: tr::lng_credits_box_out_video(Ui::Text::WithEntities))
				: tr::lng_credits_box_out_both(
					lt_photo,
					std::move(photosBold),
					lt_video,
					std::move(videosBold),
					Ui::Text::WithEntities);
		return tr::lng_credits_box_out_media(
			lt_count,
			rpl::single(form->invoice.amount) | tr::to_count(),
			lt_media,
			std::move(media),
			lt_chat,
			rpl::single(Ui::Text::Bold(data.peer->name())),
			Ui::Text::RichLangValue);
	}

	const auto bot = session->data().user(form->botId);
	return tr::lng_credits_box_out_sure(
		lt_count,
		rpl::single(form->invoice.amount) | tr::to_count(),
		lt_text,
		rpl::single(TextWithEntities{ form->title }),
		lt_bot,
		rpl::single(TextWithEntities{ bot->name() }),
		Ui::Text::RichLangValue);
}

[[nodiscard]] object_ptr<Ui::RpWidget> SendCreditsThumbnail(
		not_null<Ui::RpWidget*> parent,
		not_null<Main::Session*> session,
		not_null<Payments::CreditsFormData*> form,
		int photoSize) {
	if (const auto data = LookupPaidMediaData(session, form)) {
		const auto first = data.invoice->extendedMedia[0]->photo();
		const auto second = (data.photos > 1)
			? data.invoice->extendedMedia[1]->photo()
			: nullptr;
		const auto totalCount = int(data.invoice->extendedMedia.size());
		if (first && first->extendedMediaPreview()) {
			return Settings::PaidMediaThumbnail(
				parent,
				first,
				second,
				totalCount,
				photoSize);
		}
	}
	if (form->photo) {
		return Settings::HistoryEntryPhoto(parent, form->photo, photoSize);
	}
	const auto bot = session->data().user(form->botId);
	return object_ptr<Ui::UserpicButton>(
		parent,
		bot,
		st::defaultUserpicButton);
}

} // namespace

void SendCreditsBox(
		not_null<Ui::GenericBox*> box,
		std::shared_ptr<Payments::CreditsFormData> form,
		Fn<void()> sent) {
	if (!form) {
		return;
	}
	struct State {
		rpl::variable<bool> confirmButtonBusy = false;
	};
	const auto state = box->lifetime().make_state<State>();
	box->setStyle(st::giveawayGiftCodeBox);
	box->setNoContentMargin(true);

	const auto session = form->invoice.session;

	const auto photoSize = st::defaultUserpicButton.photoSize;

	const auto content = box->verticalLayout();
	Ui::AddSkip(content, photoSize / 2);

	{
		const auto ministarsContainer = Ui::CreateChild<Ui::RpWidget>(box);
		const auto fullHeight = photoSize * 2;
		using MiniStars = Ui::Premium::ColoredMiniStars;
		const auto ministars = box->lifetime().make_state<MiniStars>(
			ministarsContainer,
			false,
			Ui::Premium::MiniStars::Type::BiStars);
		ministars->setColorOverride(Ui::Premium::CreditsIconGradientStops());

		ministarsContainer->paintRequest(
		) | rpl::start_with_next([=] {
			auto p = QPainter(ministarsContainer);
			ministars->paint(p);
		}, ministarsContainer->lifetime());

		box->widthValue(
		) | rpl::start_with_next([=](int width) {
			ministarsContainer->resize(width, fullHeight);
			const auto w = fullHeight / 3 * 2;
			ministars->setCenter(QRect(
				(width - w) / 2,
				(fullHeight - w) / 2,
				w,
				w));
		}, ministarsContainer->lifetime());
	}

	const auto thumb = box->addRow(object_ptr<Ui::CenterWrap<>>(
		content,
		SendCreditsThumbnail(content, session, form.get(), photoSize)));
	thumb->setAttribute(Qt::WA_TransparentForMouseEvents);

	Ui::AddSkip(content);
	box->addRow(object_ptr<Ui::CenterWrap<>>(
		box,
		object_ptr<Ui::FlatLabel>(
			box,
			tr::lng_credits_box_out_title(),
			st::settingsPremiumUserTitle)));
	Ui::AddSkip(content);
	box->addRow(object_ptr<Ui::CenterWrap<>>(
		box,
		object_ptr<Ui::FlatLabel>(
			box,
			SendCreditsConfirmText(session, form.get()),
			st::creditsBoxAbout)));
	Ui::AddSkip(content);
	Ui::AddSkip(content);

	const auto button = box->addButton(rpl::single(QString()), [=] {
		if (state->confirmButtonBusy.current()) {
			return;
		}
		state->confirmButtonBusy = true;
		session->api().request(
			MTPpayments_SendStarsForm(
				MTP_flags(0),
				MTP_long(form->formId),
				form->inputInvoice)
		).done([=](auto result) {
			state->confirmButtonBusy = false;
			box->closeBox();
			sent();
		}).fail([=](const MTP::Error &error) {
			state->confirmButtonBusy = false;
			box->uiShow()->showToast(error.type());
		}).send();
	});
	{
		using namespace Info::Statistics;
		const auto loadingAnimation = InfiniteRadialAnimationWidget(
			button,
			st::giveawayGiftCodeStartButton.height / 2);
		AddChildToWidgetCenter(button.data(), loadingAnimation);
		loadingAnimation->showOn(state->confirmButtonBusy.value());
		}
	{
		auto buttonText = tr::lng_credits_box_out_confirm(
			lt_count,
			rpl::single(form->invoice.amount) | tr::to_count(),
			lt_emoji,
			rpl::single(CreditsEmojiSmall(session)),
			Ui::Text::RichLangValue);
		const auto buttonLabel = Ui::CreateChild<Ui::FlatLabel>(
			button,
			rpl::single(QString()),
			st::creditsBoxButtonLabel);
		std::move(
			buttonText
		) | rpl::start_with_next([=](const TextWithEntities &text) {
			buttonLabel->setMarkedText(
				text,
				Core::MarkedTextContext{
					.session = session,
					.customEmojiRepaint = [=] { buttonLabel->update(); },
				});
		}, buttonLabel->lifetime());
		buttonLabel->setTextColorOverride(
			box->getDelegate()->style().button.textFg->c);
		button->sizeValue(
		) | rpl::start_with_next([=](const QSize &size) {
			buttonLabel->moveToLeft(
				(size.width() - buttonLabel->width()) / 2,
				(size.height() - buttonLabel->height()) / 2);
		}, buttonLabel->lifetime());
		buttonLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
		state->confirmButtonBusy.value(
		) | rpl::start_with_next([=](bool busy) {
			buttonLabel->setVisible(!busy);
		}, buttonLabel->lifetime());
	}

	const auto buttonWidth = st::boxWidth
		- rect::m::sum::h(st::giveawayGiftCodeBox.buttonPadding);
	button->widthValue() | rpl::filter([=] {
		return (button->widthNoMargins() != buttonWidth);
	}) | rpl::start_with_next([=] {
		button->resizeToWidth(buttonWidth);
	}, button->lifetime());

	{
		const auto close = Ui::CreateChild<Ui::IconButton>(
			box.get(),
			st::boxTitleClose);
		close->setClickedCallback([=] {
			box->closeBox();
		});
		box->widthValue(
		) | rpl::start_with_next([=](int width) {
			close->moveToRight(0, 0);
			close->raise();
		}, close->lifetime());
	}

	{
		const auto balance = Settings::AddBalanceWidget(
			content,
			session->creditsValue(),
			false);
		const auto api = balance->lifetime().make_state<Api::CreditsStatus>(
			session->user());
		api->request({}, [=](Data::CreditsStatusSlice slice) {
			session->setCredits(slice.balance);
		});
		rpl::combine(
			balance->sizeValue(),
			content->sizeValue()
		) | rpl::start_with_next([=](const QSize &, const QSize &) {
			balance->moveToLeft(
				st::creditsHistoryRightSkip * 2,
				st::creditsHistoryRightSkip);
			balance->update();
		}, balance->lifetime());
	}
}

TextWithEntities CreditsEmoji(not_null<Main::Session*> session) {
	return Ui::Text::SingleCustomEmoji(
		session->data().customEmojiManager().registerInternalEmoji(
			st::settingsPremiumIconStar,
			QMargins{ 0, -st::moderateBoxExpandInnerSkip, 0, 0 },
			true),
		QString(QChar(0x2B50)));
}

TextWithEntities CreditsEmojiSmall(not_null<Main::Session*> session) {
	return Ui::Text::SingleCustomEmoji(
		session->data().customEmojiManager().registerInternalEmoji(
			st::starIconSmall,
			st::starIconSmallPadding,
			true),
		QString(QChar(0x2B50)));
}

} // namespace Ui
