/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

#include "data/data_peer.h"

namespace Data {
struct Reaction;
struct AllowedReactions;
} // namespace Data

namespace Ui {
class GenericBox;
} // namespace Ui

namespace Window {
class SessionNavigation;
} // namespace Window

struct EditAllowedReactionsArgs {
	not_null<Window::SessionNavigation*> navigation;
	int allowedCustomReactions = 0;
	int customReactionsHardLimit = 0;
	bool isGroup = false;
	std::vector<Data::Reaction> list;
	Data::AllowedReactions allowed;
	Fn<void(int required)> askForBoosts;
	Fn<void(const Data::AllowedReactions &)> save;
};

void EditAllowedReactionsBox(
	not_null<Ui::GenericBox*> box,
	EditAllowedReactionsArgs &&args);

void SaveAllowedReactions(
	not_null<PeerData*> peer,
	const Data::AllowedReactions &allowed);
