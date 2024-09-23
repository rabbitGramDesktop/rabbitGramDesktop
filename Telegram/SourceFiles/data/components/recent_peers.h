/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

namespace Main {
class Session;
} // namespace Main

namespace Data {

class RecentPeers final {
public:
	explicit RecentPeers(not_null<Main::Session*> session);
	~RecentPeers();

	[[nodiscard]] const std::vector<not_null<PeerData*>> &list() const;
	[[nodiscard]] rpl::producer<> updates() const;

	void remove(not_null<PeerData*> peer);
	void bump(not_null<PeerData*> peer);
	void clear();

	[[nodiscard]] QByteArray serialize() const;
	void applyLocal(QByteArray serialized);

private:
	const not_null<Main::Session*> _session;

	std::vector<not_null<PeerData*>> _list;
	rpl::event_stream<> _updates;

};

} // namespace Data
