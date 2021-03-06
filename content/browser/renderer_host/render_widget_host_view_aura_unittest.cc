// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/browser/renderer_host/render_widget_host_view_aura.h"

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "content/browser/renderer_host/test_render_view_host.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/aura/client/aura_constants.h"
#include "ui/aura/window.h"
#include "ui/base/ui_base_types.h"

using content::RenderWidgetHostView;
using content::RenderViewHostImplTestHarness;

// This approach (of using RenderViewHostImplTestHarness's
// RenderViewHost for a new RenderWidgetHostView) is borrowed from
// RenderWidgetHostViewMacTest.
class RenderWidgetHostViewAuraTest : public RenderViewHostImplTestHarness {
 public:
  RenderWidgetHostViewAuraTest() : old_rwhv_(NULL) {}

  virtual void SetUp() {
    RenderViewHostImplTestHarness::SetUp();
    old_rwhv_ = rvh()->GetView();
    rwhv_aura_ = static_cast<RenderWidgetHostViewAura*>(
        RenderWidgetHostView::CreateViewForWidget(rvh()));
  }

  virtual void TearDown() {
    aura::Window* window = rwhv_aura_->GetNativeView();
    if (window->parent())
      window->parent()->RemoveChild(window);
    rwhv_aura_->Destroy();
    // Destroying RWHV sets the host's view to NULL, so destroying view first,
    // then set the view.
    test_rvh()->SetView(old_rwhv_);
    RenderViewHostImplTestHarness::TearDown();
  }

 protected:
  RenderWidgetHostViewAura* rwhv_aura_;

 private:
  RenderWidgetHostView* old_rwhv_;

  DISALLOW_COPY_AND_ASSIGN(RenderWidgetHostViewAuraTest);
};

// Checks that a fullscreen view has the correct show-state and receives the
// focus.
TEST_F(RenderWidgetHostViewAuraTest, Fullscreen) {
  rwhv_aura_->InitAsFullscreen(NULL);

  aura::Window* window = rwhv_aura_->GetNativeView();
  ASSERT_TRUE(window != NULL);
  EXPECT_EQ(ui::SHOW_STATE_FULLSCREEN,
            window->GetProperty(aura::client::kShowStateKey));

  // Check that we requested and received the focus.
  EXPECT_TRUE(window->HasFocus());

  // Check that we'll also say it's okay to activate the window when there's an
  // ActivationClient defined.
  EXPECT_TRUE(rwhv_aura_->ShouldActivate(NULL));
}
