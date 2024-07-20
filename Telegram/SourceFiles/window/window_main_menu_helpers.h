/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

namespace Ui {
class FlatLabel;
class RpWidget;
class SettingsButton;
class VerticalLayout;
} // namespace Ui

namespace Window {

class SessionController;

[[nodiscard]] not_null<Ui::FlatLabel*> AddVersionLabel(
	not_null<Ui::RpWidget*>);

[[nodiscard]] not_null<Ui::SettingsButton*> AddMyChannelsBox(
	not_null<Ui::SettingsButton*> button,
	not_null<SessionController*> controller,
	bool chats);

void SetupMenuBots(
	not_null<Ui::VerticalLayout*> container,
	not_null<SessionController*> controller);

} // namespace Window
