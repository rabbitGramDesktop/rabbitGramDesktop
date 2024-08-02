/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

class DocumentData;

namespace Data {
struct FileOrigin;
} // namespace Data

namespace Main {
class Session;
} // namespace Main

namespace Stickers {

class GiftBoxPack final {
public:
	explicit GiftBoxPack(not_null<Main::Session*> session);
	~GiftBoxPack();

	void load();
	[[nodiscard]] int monthsForStars(int stars) const;
	[[nodiscard]] DocumentData *lookup(int months) const;
	[[nodiscard]] Data::FileOrigin origin() const;

private:
	using SetId = uint64;
	void applySet(const MTPDmessages_stickerSet &data);

	const not_null<Main::Session*> _session;
	const std::vector<int> _localMonths;

	std::vector<DocumentData*> _documents;
	SetId _setId = 0;
	uint64 _accessHash = 0;
	mtpRequestId _requestId = 0;

};

} // namespace Stickers
