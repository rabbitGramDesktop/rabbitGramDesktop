/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

namespace Main {
class Session;
} // namespace Main

class History;
class PhotoData;
class DocumentData;
struct FilePrepareResult;

namespace Api {

struct MessageToSend;
struct SendAction;

void SendExistingDocument(
	MessageToSend &&message,
	not_null<DocumentData*> document,
	std::optional<MsgId> localMessageId = std::nullopt);

void SendExistingPhoto(
	MessageToSend &&message,
	not_null<PhotoData*> photo,
	std::optional<MsgId> localMessageId = std::nullopt);

bool SendDice(MessageToSend &message);

void FillMessagePostFlags(
	const SendAction &action,
	not_null<PeerData*> peer,
	MessageFlags &flags);

void SendConfirmedFile(
	not_null<Main::Session*> session,
	const std::shared_ptr<FilePrepareResult> &file);

} // namespace Api
