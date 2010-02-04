// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_APPCACHE_CHROME_APPCACHE_SERVICE_H_
#define CHROME_COMMON_APPCACHE_CHROME_APPCACHE_SERVICE_H_

#include "base/ref_counted.h"
#include "chrome/browser/host_content_settings_map.h"
#include "chrome/common/notification_registrar.h"
#include "webkit/appcache/appcache_service.h"

class ChromeURLRequestContext;
class FilePath;

// An AppCacheService subclass used by the chrome. There is an instance
// associated with each Profile. This derivation adds refcounting semantics
// since a profile has multiple URLRequestContexts which refer to the same
// object, and those URLRequestContexts are refcounted independently of the
// owning profile.
//
// All methods, including the ctor and dtor, are expected to be called on
// the IO thread.
class ChromeAppCacheService
    : public base::RefCounted<ChromeAppCacheService>,
      public appcache::AppCacheService,
      public NotificationObserver {
 public:
  ChromeAppCacheService(const FilePath& profile_path,
                        ChromeURLRequestContext* request_context);

  static void ClearLocalState(const FilePath& profile_path);

 private:
  friend class base::RefCounted<ChromeAppCacheService>;
  virtual ~ChromeAppCacheService();

  // NotificationObserver override
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  scoped_refptr<HostContentSettingsMap> host_contents_settings_map_;
  NotificationRegistrar registrar_;
};

#endif  // CHROME_COMMON_APPCACHE_CHROME_APPCACHE_SERVICE_H_
