/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

namespace ChatHelpers {

struct ComposeFeatures {
	bool likes = false;
	bool sendAs = true;
	bool ttlInfo = true;
	bool botCommandSend = true;
	bool silentBroadcastToggle = true;
	bool attachBotsMenu = true;
	bool inlineBots = true;
	bool megagroupSet = true;
	bool stickersSettings = true;
	bool openStickerSets = true;
	bool autocompleteHashtags = true;
	bool autocompleteMentions = true;
	bool autocompleteCommands = true;
	bool commonTabbedPanel = true;
};

} // namespace ChatHelpers
