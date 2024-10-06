/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

#include "base/expected.h"
#include "base/object_ptr.h"
#include "base/weak_ptr.h"
#include "base/flags.h"
#include "ui/rect_part.h"
#include "ui/round_rect.h"
#include "webview/webview_common.h"

class QJsonObject;
class QJsonValue;

namespace Ui {
class BoxContent;
class RpWidget;
class SeparatePanel;
enum class LayerOption;
using LayerOptions = base::flags<LayerOption>;
} // namespace Ui

namespace Webview {
struct Available;
} // namespace Webview

namespace Ui::BotWebView {

[[nodiscard]] TextWithEntities ErrorText(const Webview::Available &info);

enum class MenuButton {
	None               = 0x00,
	OpenBot            = 0x01,
	RemoveFromMenu     = 0x02,
	RemoveFromMainMenu = 0x04,
	ShareGame          = 0x08,
};
inline constexpr bool is_flag_type(MenuButton) { return true; }
using MenuButtons = base::flags<MenuButton>;

using CustomMethodResult = base::expected<QByteArray, QString>;
struct CustomMethodRequest {
	QString method;
	QByteArray params;
	Fn<void(CustomMethodResult)> callback;
};

class Delegate {
public:
	virtual Webview::ThemeParams botThemeParams() = 0;
	virtual bool botHandleLocalUri(QString uri, bool keepOpen) = 0;
	virtual void botHandleInvoice(QString slug) = 0;
	virtual void botHandleMenuButton(MenuButton button) = 0;
	virtual bool botValidateExternalLink(QString uri) = 0;
	virtual void botOpenIvLink(QString uri) = 0;
	virtual void botSendData(QByteArray data) = 0;
	virtual void botSwitchInlineQuery(
		std::vector<QString> chatTypes,
		QString query) = 0;
	virtual void botCheckWriteAccess(Fn<void(bool allowed)> callback) = 0;
	virtual void botAllowWriteAccess(Fn<void(bool allowed)> callback) = 0;
	virtual void botSharePhone(Fn<void(bool shared)> callback) = 0;
	virtual void botInvokeCustomMethod(CustomMethodRequest request) = 0;
	virtual void botOpenPrivacyPolicy() = 0;
	virtual void botClose() = 0;
};

class Panel final : public base::has_weak_ptr {
public:
	Panel(
		const Webview::StorageId &storageId,
		rpl::producer<QString> title,
		object_ptr<Ui::RpWidget> titleBadge,
		not_null<Delegate*> delegate,
		MenuButtons menuButtons,
		bool allowClipboardRead);
	~Panel();

	void requestActivate();
	void toggleProgress(bool shown);

	bool showWebview(
		const QString &url,
		const Webview::ThemeParams &params,
		rpl::producer<QString> bottomText);

	void showBox(object_ptr<BoxContent> box);
	void showBox(
		object_ptr<BoxContent> box,
		LayerOptions options,
		anim::type animated);
	void hideLayer(anim::type animated);
	void showToast(TextWithEntities &&text);
	not_null<QWidget*> toastParent() const;
	void showCriticalError(const TextWithEntities &text);
	void showWebviewError(
		const QString &text,
		const Webview::Available &information);

	void updateThemeParams(const Webview::ThemeParams &params);

	void hideForPayment();
	void invoiceClosed(const QString &slug, const QString &status);

	[[nodiscard]] rpl::lifetime &lifetime();

private:
	class Button;
	struct Progress;
	struct WebviewWithLifetime;

	bool createWebview(const Webview::ThemeParams &params);
	void createWebviewBottom();
	void showWebviewProgress();
	void hideWebviewProgress();
	void setTitle(rpl::producer<QString> title);
	void sendDataMessage(const QJsonObject &args);
	void switchInlineQueryMessage(const QJsonObject &args);
	void processButtonMessage(
		std::unique_ptr<Button> &button,
		const QJsonObject &args);
	void processBackButtonMessage(const QJsonObject &args);
	void processSettingsButtonMessage(const QJsonObject &args);
	void processHeaderColor(const QJsonObject &args);
	void processBottomBarColor(const QJsonObject &args);
	void openTgLink(const QJsonObject &args);
	void openExternalLink(const QJsonObject &args);
	void openInvoice(const QJsonObject &args);
	void openPopup(const QJsonObject &args);
	void openScanQrPopup(const QJsonObject &args);
	void openShareStory(const QJsonObject &args);
	void requestWriteAccess();
	void replyRequestWriteAccess(bool allowed);
	void requestPhone();
	void replyRequestPhone(bool shared);
	void invokeCustomMethod(const QJsonObject &args);
	void replyCustomMethod(QJsonValue requestId, QJsonObject response);
	void requestClipboardText(const QJsonObject &args);
	void setupClosingBehaviour(const QJsonObject &args);
	void createButton(std::unique_ptr<Button> &button);
	void scheduleCloseWithConfirmation();
	void closeWithConfirmation();
	void sendViewport();

	using EventData = std::variant<QString, QJsonObject>;
	void postEvent(const QString &event);
	void postEvent(const QString &event, EventData data);

	[[nodiscard]] bool allowOpenLink() const;
	[[nodiscard]] bool allowClipboardQuery() const;
	[[nodiscard]] bool progressWithBackground() const;
	[[nodiscard]] QRect progressRect() const;
	void setupProgressGeometry();
	void layoutButtons();

	Webview::StorageId _storageId;
	const not_null<Delegate*> _delegate;
	bool _closeNeedConfirmation = false;
	bool _hasSettingsButton = false;
	MenuButtons _menuButtons = {};
	std::unique_ptr<SeparatePanel> _widget;
	std::unique_ptr<WebviewWithLifetime> _webview;
	std::unique_ptr<RpWidget> _webviewBottom;
	rpl::variable<QString> _bottomText;
	QPointer<RpWidget> _webviewParent;
	std::unique_ptr<RpWidget> _bottomButtonsBg;
	std::unique_ptr<Button> _mainButton;
	std::unique_ptr<Button> _secondaryButton;
	RectPart _secondaryPosition = RectPart::Left;
	rpl::variable<int> _footerHeight = 0;
	std::unique_ptr<Progress> _progress;
	rpl::event_stream<> _themeUpdateForced;
	std::optional<QColor> _bottomBarColor;
	rpl::lifetime _headerColorLifetime;
	rpl::lifetime _bottomBarColorLifetime;
	bool _webviewProgress = false;
	bool _themeUpdateScheduled = false;
	bool _hiddenForPayment = false;
	bool _closeWithConfirmationScheduled = false;
	bool _allowClipboardRead = false;
	bool _inBlockingRequest = false;

};

struct Args {
	QString url;
	Webview::StorageId storageId;
	rpl::producer<QString> title;
	object_ptr<Ui::RpWidget> titleBadge = { nullptr };
	rpl::producer<QString> bottom;
	not_null<Delegate*> delegate;
	MenuButtons menuButtons;
	bool allowClipboardRead = false;
};
[[nodiscard]] std::unique_ptr<Panel> Show(Args &&args);

} // namespace Ui::BotWebView
