/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

class PeerData;

namespace Data {
struct BoostPrepaidGiveaway;
} // namespace Data

namespace Window {
class SessionNavigation;
} // namespace Window

namespace Ui {
class GenericBox;
} // namespace Ui

void CreateGiveawayBox(
	not_null<Ui::GenericBox*> box,
	not_null<Window::SessionNavigation*> navigation,
	not_null<PeerData*> peer,
	Fn<void()> reloadOnDone,
	std::optional<Data::BoostPrepaidGiveaway> prepaidGiveaway);
