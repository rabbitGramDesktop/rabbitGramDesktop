/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

#include "data/data_abstract_sparse_ids.h"

class PeerData;

namespace Main {
class Session;
} // namespace Main

namespace Data {

using StoriesIdsSlice = AbstractSparseIds<std::vector<StoryId>>;

[[nodiscard]] rpl::producer<StoriesIdsSlice> SavedStoriesIds(
	not_null<PeerData*> peer,
	StoryId aroundId,
	int limit);

[[nodiscard]] rpl::producer<StoriesIdsSlice> ArchiveStoriesIds(
	not_null<PeerData*> peer,
	StoryId aroundId,
	int limit);

} // namespace Data
