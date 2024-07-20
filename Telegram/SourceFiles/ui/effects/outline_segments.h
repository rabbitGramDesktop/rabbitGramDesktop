/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

namespace Ui {

inline constexpr auto kOutlineSegmentsMax = 50;

struct OutlineSegment {
	QBrush brush;
	float64 width = 0.;
};

void PaintOutlineSegments(
	QPainter &p,
	QRectF ellipse,
	const std::vector<OutlineSegment> &segments,
	float64 fromFullProgress = 1.);

[[nodiscard]] QLinearGradient UnreadStoryOutlineGradient(QRectF rect = {});

} // namespace Ui
