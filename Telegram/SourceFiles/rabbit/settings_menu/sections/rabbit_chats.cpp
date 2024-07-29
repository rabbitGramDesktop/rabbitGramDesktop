/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#include <ui/boxes/single_choice_box.h>

#include "rabbit/settings/rabbit_settings.h"
#include "rabbit/lang/rabbit_lang.h"
#include "rabbit/settings_menu/sections/rabbit_chats.h"
#include "rabbit/ui/settings/previews.h"

#include "lang_auto.h"
#include "mainwindow.h"
#include "settings/settings_common.h"
#include "ui/wrap/vertical_layout.h"
#include "ui/wrap/slide_wrap.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/labels.h"
#include "ui/widgets/checkbox.h"
#include "ui/vertical_list.h"
#include "boxes/connection_box.h"
#include "platform/platform_specific.h"
#include "window/window_session_controller.h"
#include "lang/lang_instance.h"
#include "core/application.h"
#include "storage/localstorage.h"
#include "data/data_session.h"
#include "main/main_session.h"
#include "styles/style_settings.h"
#include "styles/style_layers.h"
#include "styles/style_menu_icons.h"
#include "apiwrap.h"
#include "api/api_blocked_peers.h"
#include "ui/widgets/continuous_sliders.h"

#define SettingsMenuJsonSwitch(LangKey, Option) container->add(object_ptr<Button>( \
	container, \
	rktr(#LangKey), \
	st::settingsButtonNoIcon \
))->toggleOn( \
	rpl::single(RabbitSettings::JsonSettings::GetBool(#Option)) \
)->toggledValue( \
) | rpl::filter([](bool enabled) { \
	return (enabled != RabbitSettings::JsonSettings::GetBool(#Option)); \
}) | rpl::start_with_next([](bool enabled) { \
	RabbitSettings::JsonSettings::Set(#Option, enabled); \
	RabbitSettings::JsonSettings::Write(); \
}, container->lifetime());

namespace Settings {

	rpl::producer<QString> RabbitChats::title() {
		return rktr("rtg_settings_chats");
	}

	RabbitChats::RabbitChats(
			QWidget *parent,
			not_null<Window::SessionController *> controller)
			: Section(parent) {
		setupContent(controller);
	}

	void RabbitChats::SetupChats(not_null<Ui::VerticalLayout *> container) {
		Ui::AddSubsectionTitle(container, rktr("rtg_settings_chats"));

		const auto stickerPreview = container->add(
			object_ptr<StickerSizePreview>(container),
			st::defaultSubsectionTitlePadding);

		const auto stickerSizeLabel = container->add(
			object_ptr<Ui::LabelSimple>(
				container,
				st::settingsAudioVolumeLabel),
			st::settingsAudioVolumeLabelPadding);
		const auto stickerSizeSlider = container->add(
			object_ptr<Ui::MediaSlider>(
				container,
				st::settingsAudioVolumeSlider),
			st::settingsAudioVolumeSliderPadding);
		const auto updateStickerSizeLabel = [=](int value) {
			const auto pixels = QString::number(value);
			stickerSizeLabel->setText(ktr("rtg_chats_sticker_size", { "pixels", pixels }));
		};
		const auto updateStickerSize = [=](int value) {
			updateStickerSizeLabel(value);
			stickerPreview->repaint();
			RabbitSettings::JsonSettings::Set("sticker_size", value);
			RabbitSettings::JsonSettings::Write();
		};
		stickerSizeSlider->resize(st::settingsAudioVolumeSlider.seekSize);
		stickerSizeSlider->setPseudoDiscrete(
			193,
			[](int val) { return val + 64; },
			RabbitSettings::JsonSettings::GetInt("sticker_size"),
			updateStickerSize);
		updateStickerSizeLabel(RabbitSettings::JsonSettings::GetInt("sticker_size"));

		const auto recentStickersLimitLabel = container->add(
			object_ptr<Ui::LabelSimple>(
				container,
				st::settingsAudioVolumeLabel),
			st::settingsAudioVolumeLabelPadding);
		const auto recentStickersLimitSlider = container->add(
			object_ptr<Ui::MediaSlider>(
				container,
				st::settingsAudioVolumeSlider),
			st::settingsAudioVolumeSliderPadding);
		const auto updateRecentStickersLimitLabel = [=](int value) {
			recentStickersLimitLabel->setText(
				(value == 0)
					? ktr("rtg_chats_recent_stickers_limit_none")
					: ktr("rtg_chats_recent_stickers_limit", value, { "count", QString::number(value) }) );
		};
		const auto updateRecentStickersLimitHeight = [=](int value) {
			updateRecentStickersLimitLabel(value);
			RabbitSettings::JsonSettings::Set("recent_stickers_limit", value);
			RabbitSettings::JsonSettings::Write();
		};
		recentStickersLimitSlider->resize(st::settingsAudioVolumeSlider.seekSize);
		recentStickersLimitSlider->setPseudoDiscrete(
			201,
			[](int val) { return val; },
			RabbitSettings::JsonSettings::GetInt("recent_stickers_limit"),
			updateRecentStickersLimitHeight);
		updateRecentStickersLimitLabel(RabbitSettings::JsonSettings::GetInt("recent_stickers_limit"));

		AddButtonWithIcon(
			container,
			rktr("rtg_show_seconds"),
			st::settingsButton,
			IconDescriptor{ &st::menuIconReschedule }
		)->toggleOn(
			rpl::single(RabbitSettings::JsonSettings::GetBool("show_seconds"))
		)->toggledValue(
		) | rpl::filter([](bool enabled) {
			return (enabled != RabbitSettings::JsonSettings::GetBool("show_seconds"));
		}) | rpl::start_with_next([](bool enabled) {
			RabbitSettings::JsonSettings::Set("show_seconds", enabled);
			RabbitSettings::JsonSettings::Write();
		}, container->lifetime());

		SettingsMenuJsonSwitch(rtg_show_actions_time, show_actions_time);
		SettingsMenuJsonSwitch(rtg_comma_after_mention, comma_after_mention);
	}

	void RabbitChats::SetupRabbitChats(not_null<Ui::VerticalLayout *> container, not_null<Window::SessionController *> controller) {
		Ui::AddSkip(container);
		SetupChats(container);
	}

	void RabbitChats::setupContent(not_null<Window::SessionController *> controller) {
		const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);

		SetupRabbitChats(content, controller);

		Ui::ResizeFitChild(this, content);
	}
} // namespace Settings
