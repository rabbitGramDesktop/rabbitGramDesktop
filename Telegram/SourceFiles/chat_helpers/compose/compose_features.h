/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

namespace ChatHelpers {

struct ComposeFeatures {
	bool likes : 1 = false;
	bool sendAs : 1 = true;
	bool ttlInfo : 1 = true;
	bool botCommandSend : 1 = true;
	bool silentBroadcastToggle : 1 = true;
	bool attachBotsMenu : 1 = true;
	bool inlineBots : 1 = true;
	bool megagroupSet : 1 = true;
	bool stickersSettings : 1 = true;
	bool openStickerSets : 1 = true;
	bool autocompleteHashtags : 1 = true;
	bool autocompleteMentions : 1 = true;
	bool autocompleteCommands : 1 = true;
	bool suggestStickersByEmoji : 1 = true;
	bool commonTabbedPanel : 1 = true;
};

} // namespace ChatHelpers
