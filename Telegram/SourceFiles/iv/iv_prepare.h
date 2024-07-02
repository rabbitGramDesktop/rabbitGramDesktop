/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

namespace Iv {

struct Options;
struct Prepared;

struct Source {
	uint64 pageId = 0;
	MTPPage page;
	std::optional<MTPPhoto> webpagePhoto;
	std::optional<MTPDocument> webpageDocument;
	QString name;
};

[[nodiscard]] Prepared Prepare(const Source &source, const Options &options);

} // namespace Iv
