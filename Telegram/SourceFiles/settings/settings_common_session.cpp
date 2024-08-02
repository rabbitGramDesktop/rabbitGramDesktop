/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#include "settings/settings_common_session.h"

#include "settings/cloud_password/settings_cloud_password_email_confirm.h"
#include "settings/settings_chat.h"
#include "settings/settings_main.h"

namespace Settings {

bool HasMenu(Type type) {
	return (type == ::Settings::CloudPasswordEmailConfirmId())
		|| (type == Main::Id())
		|| (type == Chat::Id());
}

} // namespace Settings
