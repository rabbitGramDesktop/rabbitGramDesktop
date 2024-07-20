/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

#include <rpl/producer.h>

#include <QtCore/QVariant>
#include <QtCore/QJsonArray>

namespace RabbitSettings {

const auto DEFAULT_ICON = QString("default");
const auto ANGEL_ICON = QString("angel");
const auto COLOR_ICON = QString("color");
const auto IMPOSTOR_ICON = QString("impostor");
const auto MATRIX_ICON = QString("matrix");
const auto MOON_ICON = QString("moon");
const auto OLD_ICON = QString("old");
const auto SEASONAL_ICON = QString("seasonal");
const auto TWITCH_ICON = QString("twitch");

namespace JsonSettings {

void Start();
void Load();
void Write();
void Finish();

[[nodiscard]] QVariant Get(
	const QString &key,
	uint64 accountId = 0,
	bool isTestAccount = false);
[[nodiscard]] QVariant GetWithPending(
	const QString &key,
	uint64 accountId = 0,
	bool isTestAccount = false);
[[nodiscard]] rpl::producer<QString> Events(
	const QString &key,
	uint64 accountId = 0,
	bool isTestAccount = false);
[[nodiscard]] rpl::producer<QString> EventsWithPending(
	const QString &key,
	uint64 accountId = 0,
	bool isTestAccount = false);
void Set(
	const QString &key,
	QVariant value,
	uint64 accountId = 0,
	bool isTestAccount = false);
void SetAfterRestart(
	const QString &key,
	QVariant value,
	uint64 accountId = 0,
	bool isTestAccount = false);
void Reset(
	const QString &key,
	uint64 accountId = 0,
	bool isTestAccount = false);
void ResetAfterRestart(
	const QString &key,
	uint64 accountId = 0,
	bool isTestAccount = false);

inline bool GetBool(
	const QString &key,
	uint64 accountId = 0,
	bool isTestAccount = false) {
	return Get(key, accountId, isTestAccount).toBool();
}

inline int GetInt(
	const QString &key,
	uint64 accountId = 0,
	bool isTestAccount = false) {
	return Get(key, accountId, isTestAccount).toInt();
}

inline QString GetString(
	const QString &key,
	uint64 accountId = 0,
	bool isTestAccount = false) {
	return Get(key, accountId, isTestAccount).toString();
}

inline QJsonArray GetJsonArray(
	const QString &key,
	uint64 accountId = 0,
	bool isTestAccount = false) {
	return Get(key, accountId, isTestAccount).toJsonArray();
}

inline bool GetBoolWithPending(
	const QString &key,
	uint64 accountId = 0,
	bool isTestAccount = false) {
	return GetWithPending(key, accountId, isTestAccount).toBool();
}

inline int GetIntWithPending(
	const QString &key,
	uint64 accountId = 0,
	bool isTestAccount = false) {
	return GetWithPending(key, accountId, isTestAccount).toInt();
}

inline QString GetStringWithPending(
	const QString &key,
	uint64 accountId = 0,
	bool isTestAccount = false) {
	return GetWithPending(key, accountId, isTestAccount).toString();
}

inline QJsonArray GetJsonArrayWithPending(
	const QString &key,
	uint64 accountId = 0,
	bool isTestAccount = false) {
	return GetWithPending(key, accountId, isTestAccount).toJsonArray();
}

} // namespace JsonSettings
} // namespace RabbitSettings
