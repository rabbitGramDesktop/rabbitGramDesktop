/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#include <ui/boxes/single_choice_box.h>

#include "rabbit/settings/rabbit_settings.h"
#include "rabbit/lang/rabbit_lang.h"
#include "rabbit/settings_menu/sections/rabbit_appearance.h"
#include "rabbit/ui/settings/icon_picker.h"

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
	rpl::single(::RabbitSettings::JsonSettings::GetBool(#Option)) \
)->toggledValue( \
) | rpl::filter([](bool enabled) { \
	return (enabled != ::RabbitSettings::JsonSettings::GetBool(#Option)); \
}) | rpl::start_with_next([](bool enabled) { \
	::RabbitSettings::JsonSettings::Set(#Option, enabled); \
	::RabbitSettings::JsonSettings::Write(); \
}, container->lifetime());

namespace Settings {

    rpl::producer<QString> RabbitAppearance::title() {
        return rktr("rtg_settings_appearance");
    }

    RabbitAppearance::RabbitAppearance(
            QWidget *parent,
            not_null<Window::SessionController *> controller)
            : Section(parent) {
        setupContent(controller);
    }

	void RabbitAppearance::SetupAppIcon(not_null<Ui::VerticalLayout *> container) {
		Ui::AddSubsectionTitle(container, rktr("rtg_settings_app_icon"));

		container->add(
			object_ptr<IconPicker>(container),
			st::settingsCheckboxPadding);
	}

	void RabbitAppearance::SetupAppearance(not_null<Ui::VerticalLayout *> container) {
		Ui::AddSubsectionTitle(container, rktr("rtg_settings_appearance"));

		const auto userpicRoundnessLabel = container->add(
			object_ptr<Ui::LabelSimple>(
				container,
				st::settingsAudioVolumeLabel),
			st::settingsAudioVolumeLabelPadding);
    	const auto userpicRoundnessSlider = container->add(
			object_ptr<Ui::MediaSlider>(
				container,
				st::settingsAudioVolumeSlider),
			st::settingsAudioVolumeSliderPadding);
    	const auto updateUserpicRoundnessLabel = [=](int value) {
    		const auto radius = QString::number(value);
    		userpicRoundnessLabel->setText(ktr("rtg_settings_userpic_rounding", { "radius", radius }));
    	};
    	const auto updateUserpicRoundness = [=](int value) {
    		updateUserpicRoundnessLabel(value);
    		::RabbitSettings::JsonSettings::Set("userpic_roundness", value);
    		::RabbitSettings::JsonSettings::Write();
    	};
    	userpicRoundnessSlider->resize(st::settingsAudioVolumeSlider.seekSize);
    	userpicRoundnessSlider->setPseudoDiscrete(
			51,
			[](int val) { return val; },
			::RabbitSettings::JsonSettings::GetInt("userpic_roundness"),
			updateUserpicRoundness);
    	updateUserpicRoundnessLabel(::RabbitSettings::JsonSettings::GetInt("userpic_roundness"));

    	SettingsMenuJsonSwitch(rtg_general_roundness, general_roundness);
	}
	
	void RabbitAppearance::SetupSidebar(not_null<Ui::VerticalLayout  *> container) {
		Ui::AddSubsectionTitle(container, rktr("rtg_sidebar_editor"));

		AddButtonWithIcon(
			container,
			tr::lng_create_group_title(),
			st::settingsButton,
			IconDescriptor{ &st::menuIconGroups }
		)->toggleOn(
			rpl::single(::RabbitSettings::JsonSettings::GetBool("sidebar_create_group"))
		)->toggledValue(
		) | rpl::filter([](bool enabled) {
			return (enabled != ::RabbitSettings::JsonSettings::GetBool("sidebar_create_group"));
		}) | rpl::start_with_next([](bool enabled) {
			::RabbitSettings::JsonSettings::Set("sidebar_create_group", enabled);
			::RabbitSettings::JsonSettings::Write();
		}, container->lifetime());

		AddButtonWithIcon(
			container,
			tr::lng_create_channel_title(),
			st::settingsButton,
			IconDescriptor{ &st::menuIconChannel }
		)->toggleOn(
			rpl::single(::RabbitSettings::JsonSettings::GetBool("sidebar_create_channel"))
		)->toggledValue(
		) | rpl::filter([](bool enabled) {
			return (enabled != ::RabbitSettings::JsonSettings::GetBool("sidebar_create_channel"));
		}) | rpl::start_with_next([](bool enabled) {
			::RabbitSettings::JsonSettings::Set("sidebar_create_channel", enabled);
			::RabbitSettings::JsonSettings::Write();
		}, container->lifetime());

		AddButtonWithIcon(
			container,
			tr::lng_menu_my_stories(),
			st::settingsButton,
			IconDescriptor{ &st::menuIconStoriesSavedSection }
		)->toggleOn(
			rpl::single(::RabbitSettings::JsonSettings::GetBool("sidebar_stories"))
		)->toggledValue(
		) | rpl::filter([](bool enabled) {
			return (enabled != ::RabbitSettings::JsonSettings::GetBool("sidebar_stories"));
		}) | rpl::start_with_next([](bool enabled) {
			::RabbitSettings::JsonSettings::Set("sidebar_stories", enabled);
			::RabbitSettings::JsonSettings::Write();
		}, container->lifetime());

		AddButtonWithIcon(
			container,
			tr::lng_filters_type_bots(),
			st::settingsButton,
			IconDescriptor{ &st::menuIconBots }
		)->toggleOn(
			rpl::single(::RabbitSettings::JsonSettings::GetBool("sidebar_bots"))
		)->toggledValue(
		) | rpl::filter([](bool enabled) {
			return (enabled != ::RabbitSettings::JsonSettings::GetBool("sidebar_bots"));
		}) | rpl::start_with_next([](bool enabled) {
			::RabbitSettings::JsonSettings::Set("sidebar_bots", enabled);
			::RabbitSettings::JsonSettings::Write();
		}, container->lifetime());

		AddButtonWithIcon(
			container,
			tr::lng_menu_contacts(),
			st::settingsButton,
			IconDescriptor{ &st::menuIconProfile }
		)->toggleOn(
			rpl::single(::RabbitSettings::JsonSettings::GetBool("sidebar_contacts"))
		)->toggledValue(
		) | rpl::filter([](bool enabled) {
			return (enabled != ::RabbitSettings::JsonSettings::GetBool("sidebar_contacts"));
		}) | rpl::start_with_next([](bool enabled) {
			::RabbitSettings::JsonSettings::Set("sidebar_contacts", enabled);
			::RabbitSettings::JsonSettings::Write();
		}, container->lifetime());

		AddButtonWithIcon(
			container,
			tr::lng_menu_calls(),
			st::settingsButton,
			IconDescriptor{ &st::menuIconPhone }
		)->toggleOn(
			rpl::single(::RabbitSettings::JsonSettings::GetBool("sidebar_calls"))
		)->toggledValue(
		) | rpl::filter([](bool enabled) {
			return (enabled != ::RabbitSettings::JsonSettings::GetBool("sidebar_calls"));
		}) | rpl::start_with_next([](bool enabled) {
			::RabbitSettings::JsonSettings::Set("sidebar_calls", enabled);
			::RabbitSettings::JsonSettings::Write();
		}, container->lifetime());

		AddButtonWithIcon(
			container,
			tr::lng_saved_messages(),
			st::settingsButton,
			IconDescriptor{ &st::menuIconSavedMessages }
		)->toggleOn(
			rpl::single(::RabbitSettings::JsonSettings::GetBool("sidebar_saved_messages"))
		)->toggledValue(
		) | rpl::filter([](bool enabled) {
			return (enabled != ::RabbitSettings::JsonSettings::GetBool("sidebar_saved_messages"));
		}) | rpl::start_with_next([](bool enabled) {
			::RabbitSettings::JsonSettings::Set("sidebar_saved_messages", enabled);
			::RabbitSettings::JsonSettings::Write();
		}, container->lifetime());

		AddButtonWithIcon(
			container,
			tr::lng_menu_night_mode(),
			st::settingsButton,
			IconDescriptor{ &st::menuIconNightMode }
		)->toggleOn(
			rpl::single(::RabbitSettings::JsonSettings::GetBool("sidebar_night_mode"))
		)->toggledValue(
		) | rpl::filter([](bool enabled) {
			return (enabled != ::RabbitSettings::JsonSettings::GetBool("sidebar_night_mode"));
		}) | rpl::start_with_next([](bool enabled) {
			::RabbitSettings::JsonSettings::Set("sidebar_night_mode", enabled);
			::RabbitSettings::JsonSettings::Write();
		}, container->lifetime());
	}

    void RabbitAppearance::SetupRabbitAppearance(not_null<Ui::VerticalLayout *> container, not_null<Window::SessionController *> controller) {
		Ui::AddSkip(container);
		SetupAppIcon(container);

		Ui::AddSkip(container);
		Ui::AddDivider(container);
		Ui::AddSkip(container);
    	SetupAppearance(container);

		Ui::AddSkip(container);
		Ui::AddDivider(container);
		Ui::AddSkip(container);
		SetupSidebar(container);
    }

    void RabbitAppearance::setupContent(not_null<Window::SessionController *> controller) {
        const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);

        SetupRabbitAppearance(content, controller);

        Ui::ResizeFitChild(this, content);
    }
} // namespace Settings
