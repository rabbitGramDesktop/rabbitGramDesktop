/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

namespace Data {
struct StatisticalChart;
} // namespace Data

namespace Statistic {

struct Limits;

struct LeftStartAndStep final {
	float64 start = 0.;
	float64 step = 0.;
};

[[nodiscard]] LeftStartAndStep ComputeLeftStartAndStep(
	const Data::StatisticalChart &chartData,
	const Limits &xPercentageLimits,
	const QRect &rect,
	float64 xIndexStart);

[[nodiscard]] Limits FindStackXIndicesFromRawXPercentages(
	const Data::StatisticalChart &chartData,
	const Limits &rawXPercentageLimits,
	const Limits &zoomedInLimitXIndices);

} // namespace Statistic
