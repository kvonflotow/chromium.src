// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_INFOBARS_INFOBAR_DELEGATE_H_
#define CHROME_BROWSER_INFOBARS_INFOBAR_DELEGATE_H_

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "chrome/browser/infobars/infobar_service.h"
#include "ui/base/window_open_disposition.h"

class AutoLoginInfoBarDelegate;
class ConfirmInfoBarDelegate;
class ExtensionInfoBarDelegate;
class InfoBar;
class InsecureContentInfoBarDelegate;
class MediaStreamInfoBarDelegate;
class PopupBlockedInfoBarDelegate;
class RegisterProtocolHandlerInfoBarDelegate;
class ScreenCaptureInfoBarDelegate;
class ThemeInstalledInfoBarDelegate;
class ThreeDAPIInfoBarDelegate;
class TranslateInfoBarDelegate;

namespace gfx {
class Image;
}

// An interface implemented by objects wishing to control an InfoBar.
// Implementing this interface is not sufficient to use an InfoBar, since it
// does not map to a specific InfoBar type. Instead, you must implement
// ConfirmInfoBarDelegate, or override with your own delegate for your own
// InfoBar variety.
class InfoBarDelegate {
 public:
  // The type of the infobar. It controls its appearance, such as its background
  // color.
  enum Type {
    WARNING_TYPE,
    PAGE_ACTION_TYPE,
  };

  enum InfoBarAutomationType {
    CONFIRM_INFOBAR,
    PASSWORD_INFOBAR,
    RPH_INFOBAR,
    UNKNOWN_INFOBAR,
  };

  // Value to use when the InfoBar has no icon to show.
  static const int kNoIconID;

  // Called when the InfoBar that owns this delegate is being destroyed.  At
  // this point nothing is visible onscreen.
  virtual ~InfoBarDelegate();

  virtual InfoBarAutomationType GetInfoBarAutomationType() const;

  // Returns true if the supplied |delegate| is equal to this one. Equality is
  // left to the implementation to define. This function is called by the
  // InfoBarService when determining whether or not a delegate should be
  // added because a matching one already exists. If this function returns true,
  // the InfoBarService will not add the new delegate because it considers
  // one to already be present.
  virtual bool EqualsDelegate(InfoBarDelegate* delegate) const;

  // Returns true if the InfoBar should be closed automatically after the page
  // is navigated. By default this returns true if the navigation is to a new
  // page (not including reloads).  Subclasses wishing to change this behavior
  // can override either this function or ShouldExpireInternal(), depending on
  // what level of control they need.
  virtual bool ShouldExpire(const content::LoadCommittedDetails& details) const;

  // Called when the user clicks on the close button to dismiss the infobar.
  virtual void InfoBarDismissed();

  // Return the resource ID of the icon to be shown for this InfoBar.  If the
  // value is equal to |kNoIconID|, no icon is shown.
  virtual int GetIconID() const;

  // Returns the type of the infobar.  The type determines the appearance (such
  // as background color) of the infobar.
  virtual Type GetInfoBarType() const;

  // Type-checking downcast routines:
  virtual AutoLoginInfoBarDelegate* AsAutoLoginInfoBarDelegate();
  virtual ConfirmInfoBarDelegate* AsConfirmInfoBarDelegate();
  virtual ExtensionInfoBarDelegate* AsExtensionInfoBarDelegate();
  virtual InsecureContentInfoBarDelegate* AsInsecureContentInfoBarDelegate();
  virtual MediaStreamInfoBarDelegate* AsMediaStreamInfoBarDelegate();
  virtual PopupBlockedInfoBarDelegate* AsPopupBlockedInfoBarDelegate();
  virtual RegisterProtocolHandlerInfoBarDelegate*
      AsRegisterProtocolHandlerInfoBarDelegate();
  virtual ScreenCaptureInfoBarDelegate* AsScreenCaptureInfoBarDelegate();
  virtual ThemeInstalledInfoBarDelegate* AsThemePreviewInfobarDelegate();
  virtual TranslateInfoBarDelegate* AsTranslateInfoBarDelegate();

  void set_infobar(InfoBar* infobar) { infobar_ = infobar; }

  // Store the unique id for the active entry in our WebContents, to be used
  // later upon navigation to determine if this InfoBarDelegate should be
  // expired.
  void StoreActiveEntryUniqueID();

  // Return the icon to be shown for this InfoBar. If the returned Image is
  // empty, no icon is shown.
  virtual gfx::Image GetIcon() const;

  // This trivial getter is defined out-of-line in order to avoid needing to
  // #include infobar.h, which would lead to circular #includes.
  content::WebContents* web_contents();

 protected:
  InfoBarDelegate();

  // Returns true if the navigation is to a new URL or a reload occured.
  virtual bool ShouldExpireInternal(
      const content::LoadCommittedDetails& details) const;

  int contents_unique_id() const { return contents_unique_id_; }
  InfoBar* infobar() { return infobar_; }

 private:
  // The unique id of the active NavigationEntry of the WebContents that we were
  // opened for. Used to help expire on navigations.
  int contents_unique_id_;

  // The InfoBar associated with us.
  InfoBar* infobar_;

  DISALLOW_COPY_AND_ASSIGN(InfoBarDelegate);
};

#endif  // CHROME_BROWSER_INFOBARS_INFOBAR_DELEGATE_H_
