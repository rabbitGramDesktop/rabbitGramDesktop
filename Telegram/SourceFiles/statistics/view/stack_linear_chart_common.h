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
class LinesFilterController;

struct PiePartData final {
	struct Part final {
		float64 roundedPercentage = 0; // 0.XX.
		float64 stackedAngle = 0.;
		QString percentageText;
	};
	std::vector<Part> parts;
	bool pieHasSinglePart = false;
};

[[nodiscard]] PiePartData PiePartsPercentage(
	const std::vector<float64> &sums,
	float64 totalSum,
	bool round);

[[nodiscard]] PiePartData PiePartsPercentageByIndices(
	const Data::StatisticalChart &chartData,
	const std::shared_ptr<LinesFilterController> &linesFilter,
	const Limits &xIndices);

} // namespace Statistic
