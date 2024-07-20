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

[[nodiscard]] QImage ChartCurrencyIcon(
		const Data::StatisticalChart &chartData,
		std::optional<QColor> color);

} // namespace Statistic
