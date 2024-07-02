/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

class PeerData;

namespace Data {
class Thread;
class Folder;
class Forum;
class SavedSublist;
} // namespace Data

namespace Main {
class Account;
class Session;
} // namespace Main

namespace Window {

enum class SeparateType {
	Primary,
	Archive,
	Chat,
	Forum,
	SavedSublist,
};

struct SeparateId {
	SeparateId(std::nullptr_t);
	SeparateId(not_null<Main::Account*> account);
	SeparateId(SeparateType type, not_null<Main::Session*> session);
	SeparateId(SeparateType type, not_null<Data::Thread*> thread);
	SeparateId(not_null<Data::Thread*> thread);
	SeparateId(not_null<PeerData*> peer);

	SeparateType type = SeparateType::Primary;
	Main::Account *account = nullptr;
	Data::Thread *thread = nullptr; // For types except Main and Archive.

	[[nodiscard]] bool valid() const {
		return account != nullptr;
	}
	explicit operator bool() const {
		return valid();
	}

	[[nodiscard]] bool primary() const;
	[[nodiscard]] Data::Thread *chat() const;
	[[nodiscard]] Data::Forum *forum() const;
	[[nodiscard]] Data::Folder *folder() const;
	[[nodiscard]] Data::SavedSublist *sublist() const;

	[[nodiscard]] bool hasChatsList() const;

	friend inline auto operator<=>(
		const SeparateId &,
		const SeparateId &) = default;
	friend inline bool operator==(
		const SeparateId &,
		const SeparateId &) = default;
};

} // namespace Window
