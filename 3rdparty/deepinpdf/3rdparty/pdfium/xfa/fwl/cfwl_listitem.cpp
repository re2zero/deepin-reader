// Copyright 2016 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include "xfa/fwl/cfwl_listitem.h"

CFWL_ListItem::CFWL_ListItem(const CFX_WideString& text)
    : m_dwStates(0), m_wsText(text) {
  m_rtItem.Reset();
}

CFWL_ListItem::~CFWL_ListItem() {}
