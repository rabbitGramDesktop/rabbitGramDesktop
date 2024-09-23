/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

namespace Statistic {

[[nodiscard]] QString LangDayMonthYear(crl::time seconds);
[[nodiscard]] QString LangDayMonth(crl::time seconds);
[[nodiscard]] QString LangDetailedDayMonth(crl::time seconds);

} // namespace Statistic
