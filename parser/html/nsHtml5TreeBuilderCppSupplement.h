/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=2 sw=2 et tw=78: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsError.h"
#include "nsIPresShell.h"
#include "nsEventDispatcher.h"
#include "nsNodeUtils.h"
#include "nsIFrame.h"
#include "mozilla/Likely.h"

class nsPresContext;

nsHtml5TreeBuilder::nsHtml5TreeBuilder(nsHtml5OplessBuilder* aBuilder)
  : scriptingEnabled(false)
  , fragment(false)
  , contextNode(nullptr)
  , formPointer(nullptr)
  , headPointer(nullptr)
  , mBuilder(aBuilder)
  , mViewSource(nullptr)
  , mOpSink(nullptr)
  , mHandles(nullptr)
  , mHandlesUsed(0)
  , mSpeculativeLoadStage(nullptr)
  , mCurrentHtmlScriptIsAsyncOrDefer(false)
  , mPreventScriptExecution(false)
#ifdef DEBUG
  , mActive(false)
#endif
{
  MOZ_COUNT_CTOR(nsHtml5TreeBuilder);
}

nsHtml5TreeBuilder::nsHtml5TreeBuilder(nsAHtml5TreeOpSink* aOpSink,
                                       nsHtml5TreeOpStage* aStage)
  : scriptingEnabled(false)
  , fragment(false)
  , contextNode(nullptr)
  , formPointer(nullptr)
  , headPointer(nullptr)
  , mBuilder(nullptr)
  , mViewSource(nullptr)
  , mOpSink(aOpSink)
  , mHandles(new nsIContent*[NS_HTML5_TREE_BUILDER_HANDLE_ARRAY_LENGTH])
  , mHandlesUsed(0)
  , mSpeculativeLoadStage(aStage)
  , mCurrentHtmlScriptIsAsyncOrDefer(false)
  , mPreventScriptExecution(false)
#ifdef DEBUG
  , mActive(false)
#endif
{
  MOZ_COUNT_CTOR(nsHtml5TreeBuilder);
}

nsHtml5TreeBuilder::~nsHtml5TreeBuilder()
{
  MOZ_COUNT_DTOR(nsHtml5TreeBuilder);
  NS_ASSERTION(!mActive, "nsHtml5TreeBuilder deleted without ever calling end() on it!");
  mOpQueue.Clear();
}

nsIContentHandle*
nsHtml5TreeBuilder::createElement(int32_t aNamespace, nsIAtom* aName, nsHtml5HtmlAttributes* aAttributes)
{
  NS_PRECONDITION(aAttributes, "Got null attributes.");
  NS_PRECONDITION(aName, "Got null name.");
  NS_PRECONDITION(aNamespace == kNameSpaceID_XHTML || 
                  aNamespace == kNameSpaceID_SVG || 
                  aNamespace == kNameSpaceID_MathML,
                  "Bogus namespace.");

  if (mBuilder) {
    nsCOMPtr<nsIAtom> name = nsHtml5TreeOperation::Reget(aName);
    nsIContent* elem =
      nsHtml5TreeOperation::CreateElement(aNamespace,
                                          name,
                                          aAttributes,
                                          mozilla::dom::FROM_PARSER_FRAGMENT,
                                          mBuilder);
    if (MOZ_UNLIKELY(aAttributes != tokenizer->GetAttributes() &&
                     aAttributes != nsHtml5HtmlAttributes::EMPTY_ATTRIBUTES)) {
      delete aAttributes;
    }
    return elem;
  }

  nsIContentHandle* content = AllocateContentHandle();
  nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
  NS_ASSERTION(treeOp, "Tree op allocation failed.");
  treeOp->Init(aNamespace,
               aName,
               aAttributes,
               content,
               !!mSpeculativeLoadStage);
  // mSpeculativeLoadStage is non-null only in the off-the-main-thread
  // tree builder, which handles the network stream
  
  // Start wall of code for speculative loading and line numbers
  
  if (mSpeculativeLoadStage) {
    switch (aNamespace) {
      case kNameSpaceID_XHTML:
        if (nsHtml5Atoms::img == aName) {
          nsString* url = aAttributes->getValue(nsHtml5AttributeName::ATTR_SRC);
          if (url) {
            nsString* crossOrigin =
              aAttributes->getValue(nsHtml5AttributeName::ATTR_CROSSORIGIN);
            mSpeculativeLoadQueue.AppendElement()->
              InitImage(*url,
                        crossOrigin ? *crossOrigin : NullString());
          }
        } else if (nsHtml5Atoms::script == aName) {
          nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
          NS_ASSERTION(treeOp, "Tree op allocation failed.");
          treeOp->Init(eTreeOpSetScriptLineNumberAndFreeze, content, tokenizer->getLineNumber());

          nsString* url = aAttributes->getValue(nsHtml5AttributeName::ATTR_SRC);
          if (url) {
            nsString* charset = aAttributes->getValue(nsHtml5AttributeName::ATTR_CHARSET);
            nsString* type = aAttributes->getValue(nsHtml5AttributeName::ATTR_TYPE);
            nsString* crossOrigin =
              aAttributes->getValue(nsHtml5AttributeName::ATTR_CROSSORIGIN);
            mSpeculativeLoadQueue.AppendElement()->
              InitScript(*url,
                         (charset) ? *charset : EmptyString(),
                         (type) ? *type : EmptyString(),
                         (crossOrigin) ? *crossOrigin : NullString(),
                         mode == NS_HTML5TREE_BUILDER_IN_HEAD);
            mCurrentHtmlScriptIsAsyncOrDefer = 
              aAttributes->contains(nsHtml5AttributeName::ATTR_ASYNC) ||
              aAttributes->contains(nsHtml5AttributeName::ATTR_DEFER);
          }
        } else if (nsHtml5Atoms::link == aName) {
          nsString* rel = aAttributes->getValue(nsHtml5AttributeName::ATTR_REL);
          // Not splitting on space here is bogus but the old parser didn't even
          // do a case-insensitive check.
          if (rel && rel->LowerCaseEqualsASCII("stylesheet")) {
            nsString* url = aAttributes->getValue(nsHtml5AttributeName::ATTR_HREF);
            if (url) {
              nsString* charset = aAttributes->getValue(nsHtml5AttributeName::ATTR_CHARSET);
              nsString* crossOrigin =
                aAttributes->getValue(nsHtml5AttributeName::ATTR_CROSSORIGIN);
              mSpeculativeLoadQueue.AppendElement()->
                InitStyle(*url,
                          (charset) ? *charset : EmptyString(),
                          (crossOrigin) ? *crossOrigin : NullString());
            }
          }
        } else if (nsHtml5Atoms::video == aName) {
          nsString* url = aAttributes->getValue(nsHtml5AttributeName::ATTR_POSTER);
          if (url) {
            mSpeculativeLoadQueue.AppendElement()->InitImage(*url, NullString());
          }
        } else if (nsHtml5Atoms::style == aName) {
          nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
          NS_ASSERTION(treeOp, "Tree op allocation failed.");
          treeOp->Init(eTreeOpSetStyleLineNumber, content, tokenizer->getLineNumber());
        } else if (nsHtml5Atoms::html == aName) {
          nsString* url = aAttributes->getValue(nsHtml5AttributeName::ATTR_MANIFEST);
          if (url) {
            mSpeculativeLoadQueue.AppendElement()->InitManifest(*url);
          } else {
            mSpeculativeLoadQueue.AppendElement()->InitManifest(EmptyString());
          }
        } else if (nsHtml5Atoms::base == aName) {
          nsString* url =
              aAttributes->getValue(nsHtml5AttributeName::ATTR_HREF);
          if (url) {
            mSpeculativeLoadQueue.AppendElement()->InitBase(*url);
          }
        }
        break;
      case kNameSpaceID_SVG:
        if (nsHtml5Atoms::image == aName) {
          nsString* url = aAttributes->getValue(nsHtml5AttributeName::ATTR_XLINK_HREF);
          if (url) {
            mSpeculativeLoadQueue.AppendElement()->InitImage(*url, NullString());
          }
        } else if (nsHtml5Atoms::script == aName) {
          nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
          NS_ASSERTION(treeOp, "Tree op allocation failed.");
          treeOp->Init(eTreeOpSetScriptLineNumberAndFreeze, content, tokenizer->getLineNumber());

          nsString* url = aAttributes->getValue(nsHtml5AttributeName::ATTR_XLINK_HREF);
          if (url) {
            nsString* type = aAttributes->getValue(nsHtml5AttributeName::ATTR_TYPE);
            nsString* crossOrigin =
              aAttributes->getValue(nsHtml5AttributeName::ATTR_CROSSORIGIN);
            mSpeculativeLoadQueue.AppendElement()->
              InitScript(*url,
                         EmptyString(),
                         (type) ? *type : EmptyString(),
                         (crossOrigin) ? *crossOrigin : NullString(),
                         mode == NS_HTML5TREE_BUILDER_IN_HEAD);
          }
        } else if (nsHtml5Atoms::style == aName) {
          nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
          NS_ASSERTION(treeOp, "Tree op allocation failed.");
          treeOp->Init(eTreeOpSetStyleLineNumber, content, tokenizer->getLineNumber());

          nsString* url = aAttributes->getValue(nsHtml5AttributeName::ATTR_XLINK_HREF);
          if (url) {
            nsString* crossOrigin =
              aAttributes->getValue(nsHtml5AttributeName::ATTR_CROSSORIGIN);
            mSpeculativeLoadQueue.AppendElement()->
              InitStyle(*url, EmptyString(),
                        (crossOrigin) ? *crossOrigin : NullString());
          }
        }        
        break;
    }
  } else if (aNamespace != kNameSpaceID_MathML) {
    // No speculative loader--just line numbers and defer/async check
    if (nsHtml5Atoms::style == aName) {
      nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
      NS_ASSERTION(treeOp, "Tree op allocation failed.");
      treeOp->Init(eTreeOpSetStyleLineNumber, content, tokenizer->getLineNumber());
    } else if (nsHtml5Atoms::script == aName) {
      nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
      NS_ASSERTION(treeOp, "Tree op allocation failed.");
      treeOp->Init(eTreeOpSetScriptLineNumberAndFreeze, content, tokenizer->getLineNumber());
      if (aNamespace == kNameSpaceID_XHTML) {
        mCurrentHtmlScriptIsAsyncOrDefer = 
          aAttributes->contains(nsHtml5AttributeName::ATTR_SRC) &&
          (aAttributes->contains(nsHtml5AttributeName::ATTR_ASYNC) ||
           aAttributes->contains(nsHtml5AttributeName::ATTR_DEFER));
      }
    } else if (aNamespace == kNameSpaceID_XHTML && nsHtml5Atoms::html == aName) {
      nsString* url = aAttributes->getValue(nsHtml5AttributeName::ATTR_MANIFEST);
      nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
      NS_ASSERTION(treeOp, "Tree op allocation failed.");
      if (url) {
        treeOp->Init(eTreeOpProcessOfflineManifest, *url);
      } else {
        treeOp->Init(eTreeOpProcessOfflineManifest, EmptyString());
      }
    }
  }

  // End wall of code for speculative loading
  
  return content;
}

nsIContentHandle*
nsHtml5TreeBuilder::createElement(int32_t aNamespace, nsIAtom* aName, nsHtml5HtmlAttributes* aAttributes, nsIContentHandle* aFormElement)
{
  nsIContentHandle* content = createElement(aNamespace, aName, aAttributes);
  if (aFormElement) {
    if (mBuilder) {
      nsHtml5TreeOperation::SetFormElement(static_cast<nsIContent*>(content),
        static_cast<nsIContent*>(aFormElement));
    } else {
      nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
      NS_ASSERTION(treeOp, "Tree op allocation failed.");
      treeOp->Init(eTreeOpSetFormElement, content, aFormElement);
    }
  }
  return content;
}

nsIContentHandle*
nsHtml5TreeBuilder::createHtmlElementSetAsRoot(nsHtml5HtmlAttributes* aAttributes)
{
  nsIContentHandle* content = createElement(kNameSpaceID_XHTML, nsHtml5Atoms::html, aAttributes);
  if (mBuilder) {
    nsresult rv = nsHtml5TreeOperation::AppendToDocument(static_cast<nsIContent*>(content),
                                                         mBuilder);
    if (NS_FAILED(rv)) {
      MarkAsBrokenAndRequestSuspension(rv);
    }
  } else {
    nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
    NS_ASSERTION(treeOp, "Tree op allocation failed.");
    treeOp->Init(eTreeOpAppendToDocument, content);
  }
  return content;
}

void
nsHtml5TreeBuilder::detachFromParent(nsIContentHandle* aElement)
{
  NS_PRECONDITION(aElement, "Null element");

  if (mBuilder) {
    nsHtml5TreeOperation::Detach(static_cast<nsIContent*>(aElement),
                                 mBuilder);
    return;
  }

  nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
  NS_ASSERTION(treeOp, "Tree op allocation failed.");
  treeOp->Init(eTreeOpDetach, aElement);
}

void
nsHtml5TreeBuilder::appendElement(nsIContentHandle* aChild, nsIContentHandle* aParent)
{
  NS_PRECONDITION(aChild, "Null child");
  NS_PRECONDITION(aParent, "Null parent");
  if (deepTreeSurrogateParent) {
    return;
  }

  if (mBuilder) {
    nsresult rv = nsHtml5TreeOperation::Append(static_cast<nsIContent*>(aChild),
                                               static_cast<nsIContent*>(aParent),
                                               mBuilder);
    if (NS_FAILED(rv)) {
      MarkAsBrokenAndRequestSuspension(rv);
    }
    return;
  }

  nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
  NS_ASSERTION(treeOp, "Tree op allocation failed.");
  treeOp->Init(eTreeOpAppend, aChild, aParent);
}

void
nsHtml5TreeBuilder::appendChildrenToNewParent(nsIContentHandle* aOldParent, nsIContentHandle* aNewParent)
{
  NS_PRECONDITION(aOldParent, "Null old parent");
  NS_PRECONDITION(aNewParent, "Null new parent");

  if (mBuilder) {
    nsresult rv = nsHtml5TreeOperation::AppendChildrenToNewParent(
      static_cast<nsIContent*>(aOldParent),
      static_cast<nsIContent*>(aNewParent),
      mBuilder);
    if (NS_FAILED(rv)) {
      MarkAsBrokenAndRequestSuspension(rv);
    }
    return;
  }

  nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
  NS_ASSERTION(treeOp, "Tree op allocation failed.");
  treeOp->Init(eTreeOpAppendChildrenToNewParent, aOldParent, aNewParent);
}

void
nsHtml5TreeBuilder::insertFosterParentedCharacters(char16_t* aBuffer, int32_t aStart, int32_t aLength, nsIContentHandle* aTable, nsIContentHandle* aStackParent)
{
  NS_PRECONDITION(aBuffer, "Null buffer");
  NS_PRECONDITION(aTable, "Null table");
  NS_PRECONDITION(aStackParent, "Null stack parent");
  MOZ_ASSERT(!aStart, "aStart must always be zero.");

  if (mBuilder) {
    nsresult rv = nsHtml5TreeOperation::FosterParentText(
      static_cast<nsIContent*>(aStackParent),
      aBuffer, // XXX aStart always ignored???
      aLength,
      static_cast<nsIContent*>(aTable),
      mBuilder);
    if (NS_FAILED(rv)) {
      MarkAsBrokenAndRequestSuspension(rv);
    }
    return;
  }

  char16_t* bufferCopy = new char16_t[aLength];
  memcpy(bufferCopy, aBuffer, aLength * sizeof(char16_t));
  
  nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
  NS_ASSERTION(treeOp, "Tree op allocation failed.");
  treeOp->Init(eTreeOpFosterParentText, bufferCopy, aLength, aStackParent, aTable);
}

void
nsHtml5TreeBuilder::insertFosterParentedChild(nsIContentHandle* aChild, nsIContentHandle* aTable, nsIContentHandle* aStackParent)
{
  NS_PRECONDITION(aChild, "Null child");
  NS_PRECONDITION(aTable, "Null table");
  NS_PRECONDITION(aStackParent, "Null stack parent");

  if (mBuilder) {
    nsresult rv = nsHtml5TreeOperation::FosterParent(
      static_cast<nsIContent*>(aChild),
      static_cast<nsIContent*>(aStackParent),
      static_cast<nsIContent*>(aTable),
      mBuilder);
    if (NS_FAILED(rv)) {
      MarkAsBrokenAndRequestSuspension(rv);
    }
    return;
  }

  nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
  NS_ASSERTION(treeOp, "Tree op allocation failed.");
  treeOp->Init(eTreeOpFosterParent, aChild, aStackParent, aTable);
}

void
nsHtml5TreeBuilder::appendCharacters(nsIContentHandle* aParent, char16_t* aBuffer, int32_t aStart, int32_t aLength)
{
  NS_PRECONDITION(aBuffer, "Null buffer");
  NS_PRECONDITION(aParent, "Null parent");
  MOZ_ASSERT(!aStart, "aStart must always be zero.");

  if (mBuilder) {
    nsresult rv = nsHtml5TreeOperation::AppendText(
      aBuffer, // XXX aStart always ignored???
      aLength,
      static_cast<nsIContent*>(deepTreeSurrogateParent ?
                               deepTreeSurrogateParent : aParent),
      mBuilder);
    if (NS_FAILED(rv)) {
      MarkAsBrokenAndRequestSuspension(rv);
    }
    return;
  }

  char16_t* bufferCopy = new char16_t[aLength];
  memcpy(bufferCopy, aBuffer, aLength * sizeof(char16_t));
  
  nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
  NS_ASSERTION(treeOp, "Tree op allocation failed.");
  treeOp->Init(eTreeOpAppendText, bufferCopy, aLength,
      deepTreeSurrogateParent ? deepTreeSurrogateParent : aParent);
}

void
nsHtml5TreeBuilder::appendIsindexPrompt(nsIContentHandle* aParent)
{
  NS_PRECONDITION(aParent, "Null parent");

  if (mBuilder) {
    nsresult rv = nsHtml5TreeOperation::AppendIsindexPrompt(
      static_cast<nsIContent*>(aParent),
      mBuilder);
    if (NS_FAILED(rv)) {
      MarkAsBrokenAndRequestSuspension(rv);
    }
    return;
  }

  nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
  NS_ASSERTION(treeOp, "Tree op allocation failed.");
  treeOp->Init(eTreeOpAppendIsindexPrompt, aParent);
}

void
nsHtml5TreeBuilder::appendComment(nsIContentHandle* aParent, char16_t* aBuffer, int32_t aStart, int32_t aLength)
{
  NS_PRECONDITION(aBuffer, "Null buffer");
  NS_PRECONDITION(aParent, "Null parent");
  MOZ_ASSERT(!aStart, "aStart must always be zero.");

  if (deepTreeSurrogateParent) {
    return;
  }

  if (mBuilder) {
    nsresult rv = nsHtml5TreeOperation::AppendComment(
      static_cast<nsIContent*>(aParent),
      aBuffer, // XXX aStart always ignored???
      aLength,
      mBuilder);
    if (NS_FAILED(rv)) {
      MarkAsBrokenAndRequestSuspension(rv);
    }
    return;
  }

  char16_t* bufferCopy = new char16_t[aLength];
  memcpy(bufferCopy, aBuffer, aLength * sizeof(char16_t));
  
  nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
  NS_ASSERTION(treeOp, "Tree op allocation failed.");
  treeOp->Init(eTreeOpAppendComment, bufferCopy, aLength, aParent);
}

void
nsHtml5TreeBuilder::appendCommentToDocument(char16_t* aBuffer, int32_t aStart, int32_t aLength)
{
  NS_PRECONDITION(aBuffer, "Null buffer");
  MOZ_ASSERT(!aStart, "aStart must always be zero.");

  if (mBuilder) {
    nsresult rv = nsHtml5TreeOperation::AppendCommentToDocument(
      aBuffer, // XXX aStart always ignored???
      aLength,
      mBuilder);
    if (NS_FAILED(rv)) {
      MarkAsBrokenAndRequestSuspension(rv);
    }
    return;
  }

  char16_t* bufferCopy = new char16_t[aLength];
  memcpy(bufferCopy, aBuffer, aLength * sizeof(char16_t));
  
  nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
  NS_ASSERTION(treeOp, "Tree op allocation failed.");
  treeOp->Init(eTreeOpAppendCommentToDocument, bufferCopy, aLength);
}

void
nsHtml5TreeBuilder::addAttributesToElement(nsIContentHandle* aElement, nsHtml5HtmlAttributes* aAttributes)
{
  NS_PRECONDITION(aElement, "Null element");
  NS_PRECONDITION(aAttributes, "Null attributes");

  if (aAttributes == nsHtml5HtmlAttributes::EMPTY_ATTRIBUTES) {
    return;
  }

  if (mBuilder) {
    MOZ_ASSERT(aAttributes == tokenizer->GetAttributes(),
      "Using attribute other than the tokenizer's to add to body or html.");
    nsresult rv = nsHtml5TreeOperation::AddAttributes(
      static_cast<nsIContent*>(aElement),
      aAttributes,
      mBuilder);
    if (NS_FAILED(rv)) {
      MarkAsBrokenAndRequestSuspension(rv);
    }
    return;
  }

  nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
  NS_ASSERTION(treeOp, "Tree op allocation failed.");
  treeOp->Init(aElement, aAttributes);
}

void
nsHtml5TreeBuilder::markMalformedIfScript(nsIContentHandle* aElement)
{
  NS_PRECONDITION(aElement, "Null element");

  if (mBuilder) {
    // XXX innerHTML
    return;
  }

  nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
  NS_ASSERTION(treeOp, "Tree op allocation failed.");
  treeOp->Init(eTreeOpMarkMalformedIfScript, aElement);
}

void
nsHtml5TreeBuilder::start(bool fragment)
{
  mCurrentHtmlScriptIsAsyncOrDefer = false;
  deepTreeSurrogateParent = nullptr;
#ifdef DEBUG
  mActive = true;
#endif
}

void
nsHtml5TreeBuilder::end()
{
  mOpQueue.Clear();
#ifdef DEBUG
  mActive = false;
#endif
}

void
nsHtml5TreeBuilder::appendDoctypeToDocument(nsIAtom* aName, nsString* aPublicId, nsString* aSystemId)
{
  NS_PRECONDITION(aName, "Null name");

  if (mBuilder) {
    nsCOMPtr<nsIAtom> name = nsHtml5TreeOperation::Reget(aName);
    nsresult rv =
      nsHtml5TreeOperation::AppendDoctypeToDocument(name,
                                                    *aPublicId,
                                                    *aSystemId,
                                                    mBuilder);
    if (NS_FAILED(rv)) {
      MarkAsBrokenAndRequestSuspension(rv);
    }
    return;
  }

  nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
  NS_ASSERTION(treeOp, "Tree op allocation failed.");
  treeOp->Init(aName, *aPublicId, *aSystemId);
  // nsXMLContentSink can flush here, but what's the point?
  // It can also interrupt here, but we can't.
}

void
nsHtml5TreeBuilder::elementPushed(int32_t aNamespace, nsIAtom* aName, nsIContentHandle* aElement)
{
  NS_ASSERTION(aNamespace == kNameSpaceID_XHTML || aNamespace == kNameSpaceID_SVG || aNamespace == kNameSpaceID_MathML, "Element isn't HTML, SVG or MathML!");
  NS_ASSERTION(aName, "Element doesn't have local name!");
  NS_ASSERTION(aElement, "No element!");
  /*
   * The frame constructor uses recursive algorithms, so it can't deal with
   * arbitrarily deep trees. This is especially a problem on Windows where
   * the permitted depth of the runtime stack is rather small.
   *
   * The following is a protection against author incompetence--not against
   * malice. There are other ways to make the DOM deep anyway.
   *
   * The basic idea is that when the tree builder stack gets too deep,
   * append operations no longer append to the node that the HTML parsing
   * algorithm says they should but instead text nodes are append to the last
   * element that was seen before a magic tree builder stack threshold was
   * reached and element and comment nodes aren't appended to the DOM at all.
   *
   * However, for security reasons, non-child descendant text nodes inside an
   * SVG script or style element should not become children. Also, non-cell
   * table elements shouldn't be used as surrogate parents for user experience
   * reasons.
   */
  if (!deepTreeSurrogateParent && currentPtr >= MAX_REFLOW_DEPTH &&
      !(aName == nsHtml5Atoms::script ||
        aName == nsHtml5Atoms::table ||
        aName == nsHtml5Atoms::thead ||
        aName == nsHtml5Atoms::tfoot ||
        aName == nsHtml5Atoms::tbody ||
        aName == nsHtml5Atoms::tr ||
        aName == nsHtml5Atoms::colgroup ||
        aName == nsHtml5Atoms::style)) {
    deepTreeSurrogateParent = aElement;
  }
  if (aNamespace != kNameSpaceID_XHTML) {
    return;
  }
  if (aName == nsHtml5Atoms::body || aName == nsHtml5Atoms::frameset) {
    if (mBuilder) {
      // InnerHTML and DOMParser shouldn't start layout anyway
      return;
    }
    nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
    NS_ASSERTION(treeOp, "Tree op allocation failed.");
    treeOp->Init(eTreeOpStartLayout);
    return;
  }
  if (aName == nsHtml5Atoms::input ||
      aName == nsHtml5Atoms::button) {
    if (!formPointer) {
      // If form inputs don't belong to a form, their state preservation
      // won't work right without an append notification flush at this
      // point. See bug 497861.
      if (mBuilder) {
        mBuilder->FlushPendingAppendNotifications();
      } else {
        mOpQueue.AppendElement()->Init(eTreeOpFlushPendingAppendNotifications);
      }
    }
    if (mBuilder) {
      nsHtml5TreeOperation::DoneCreatingElement(static_cast<nsIContent*>(aElement));
    } else {
      mOpQueue.AppendElement()->Init(eTreeOpDoneCreatingElement, aElement);
    }
    return;
  }
  if (aName == nsHtml5Atoms::audio ||
      aName == nsHtml5Atoms::video ||
      aName == nsHtml5Atoms::menuitem) {
    if (mBuilder) {
      nsHtml5TreeOperation::DoneCreatingElement(static_cast<nsIContent*>(aElement));
    } else {
      mOpQueue.AppendElement()->Init(eTreeOpDoneCreatingElement, aElement);
    }
    return;
  }
}

void
nsHtml5TreeBuilder::elementPopped(int32_t aNamespace, nsIAtom* aName, nsIContentHandle* aElement)
{
  NS_ASSERTION(aNamespace == kNameSpaceID_XHTML || aNamespace == kNameSpaceID_SVG || aNamespace == kNameSpaceID_MathML, "Element isn't HTML, SVG or MathML!");
  NS_ASSERTION(aName, "Element doesn't have local name!");
  NS_ASSERTION(aElement, "No element!");
  if (deepTreeSurrogateParent && currentPtr <= MAX_REFLOW_DEPTH) {
    deepTreeSurrogateParent = nullptr;
  }
  if (aNamespace == kNameSpaceID_MathML) {
    return;
  }
  // we now have only SVG and HTML
  if (aName == nsHtml5Atoms::script) {
    if (mPreventScriptExecution) {
      if (mBuilder) {
        nsHtml5TreeOperation::PreventScriptExecution(static_cast<nsIContent*>(aElement));
        return;
      }
      mOpQueue.AppendElement()->Init(eTreeOpPreventScriptExecution, aElement);
      return;
    }
    if (mBuilder) {
      return;
    }
    if (mCurrentHtmlScriptIsAsyncOrDefer) {
      NS_ASSERTION(aNamespace == kNameSpaceID_XHTML, 
                   "Only HTML scripts may be async/defer.");
      nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
      NS_ASSERTION(treeOp, "Tree op allocation failed.");
      treeOp->Init(eTreeOpRunScriptAsyncDefer, aElement);      
      mCurrentHtmlScriptIsAsyncOrDefer = false;
      return;
    }
    requestSuspension();
    nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
    NS_ASSERTION(treeOp, "Tree op allocation failed.");
    treeOp->InitScript(aElement);
    return;
  }
  if (aName == nsHtml5Atoms::title) {
    if (mBuilder) {
      nsHtml5TreeOperation::DoneAddingChildren(static_cast<nsIContent*>(aElement), mBuilder);
      return;
    }
    nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
    NS_ASSERTION(treeOp, "Tree op allocation failed.");
    treeOp->Init(eTreeOpDoneAddingChildren, aElement);
    return;
  }
  if (aName == nsHtml5Atoms::style || (aNamespace == kNameSpaceID_XHTML && aName == nsHtml5Atoms::link)) {
    if (mBuilder) {
      MOZ_ASSERT(!nsContentUtils::IsSafeToRunScript(),
        "Scripts must be blocked.");
      mBuilder->FlushPendingAppendNotifications();
      mBuilder->UpdateStyleSheet(static_cast<nsIContent*>(aElement));
      return;
    }
    nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
    NS_ASSERTION(treeOp, "Tree op allocation failed.");
    treeOp->Init(eTreeOpUpdateStyleSheet, aElement);
    return;
  }
  if (aNamespace == kNameSpaceID_SVG) {
    if (mBuilder) {
      // XXX innerHTML
      // is this ever needed for the on-the-main-thread case
      return;
    }
    if (aName == nsHtml5Atoms::svg) {
      nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
      NS_ASSERTION(treeOp, "Tree op allocation failed.");
      treeOp->Init(eTreeOpSvgLoad, aElement);
    }
    return;
  }
  // we now have only HTML
  // Some HTML nodes need DoneAddingChildren() called to initialize
  // properly (e.g. form state restoration).
  // XXX expose ElementName group here and do switch
  if (aName == nsHtml5Atoms::object ||
      aName == nsHtml5Atoms::applet) {
    if (mBuilder) {
      nsHtml5TreeOperation::DoneAddingChildren(static_cast<nsIContent*>(aElement), mBuilder);
      return;
    }
    nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
    NS_ASSERTION(treeOp, "Tree op allocation failed.");
    treeOp->Init(eTreeOpDoneAddingChildren, aElement);
    return;
  }
  if (aName == nsHtml5Atoms::select || 
      aName == nsHtml5Atoms::textarea) {
    if (!formPointer) {
      // If form inputs don't belong to a form, their state preservation
      // won't work right without an append notification flush at this 
      // point. See bug 497861 and bug 539895.
      if (mBuilder) {
        mBuilder->FlushPendingAppendNotifications();
      } else {
        nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
        NS_ASSERTION(treeOp, "Tree op allocation failed.");
        treeOp->Init(eTreeOpFlushPendingAppendNotifications);
      }
    }
    if (mBuilder) {
      nsHtml5TreeOperation::DoneAddingChildren(static_cast<nsIContent*>(aElement), mBuilder);
      return;
    }
    nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
    NS_ASSERTION(treeOp, "Tree op allocation failed.");
    treeOp->Init(eTreeOpDoneAddingChildren, aElement);
    return;
  }
  if (aName == nsHtml5Atoms::meta && !fragment && !mBuilder) {
    nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
    NS_ASSERTION(treeOp, "Tree op allocation failed.");
    treeOp->Init(eTreeOpProcessMeta, aElement);
    return;
  }
  return;
}

void
nsHtml5TreeBuilder::accumulateCharacters(const char16_t* aBuf, int32_t aStart, int32_t aLength)
{
  int32_t newFillLen = charBufferLen + aLength;
  if (newFillLen > charBuffer.length) {
    int32_t newAllocLength = newFillLen + (newFillLen >> 1);
    jArray<char16_t,int32_t> newBuf = jArray<char16_t,int32_t>::newJArray(newAllocLength);
    memcpy(newBuf, charBuffer, sizeof(char16_t) * charBufferLen);
    charBuffer = newBuf;
  }
  memcpy(charBuffer + charBufferLen, aBuf + aStart, sizeof(char16_t) * aLength);
  charBufferLen = newFillLen;
}

nsIContentHandle*
nsHtml5TreeBuilder::AllocateContentHandle()
{
  if (MOZ_UNLIKELY(mBuilder)) {
    MOZ_ASSUME_UNREACHABLE("Must never allocate a handle with builder.");
    return nullptr;
  }
  if (mHandlesUsed == NS_HTML5_TREE_BUILDER_HANDLE_ARRAY_LENGTH) {
    mOldHandles.AppendElement(mHandles.forget());
    mHandles = new nsIContent*[NS_HTML5_TREE_BUILDER_HANDLE_ARRAY_LENGTH];
    mHandlesUsed = 0;
  }
#ifdef DEBUG
  mHandles[mHandlesUsed] = (nsIContent*)0xC0DEDBAD;
#endif
  return &mHandles[mHandlesUsed++];
}

bool
nsHtml5TreeBuilder::HasScript()
{
  uint32_t len = mOpQueue.Length();
  if (!len) {
    return false;
  }
  return mOpQueue.ElementAt(len - 1).IsRunScript();
}

bool
nsHtml5TreeBuilder::Flush(bool aDiscretionary)
{
  if (MOZ_UNLIKELY(mBuilder)) {
    MOZ_ASSUME_UNREACHABLE("Must never flush with builder.");
    return false;
  }
  if (!aDiscretionary ||
      !(charBufferLen &&
        currentPtr >= 0 &&
        stack[currentPtr]->isFosterParenting())) {
    // Don't flush text on discretionary flushes if the current element on
    // the stack is a foster-parenting element and there's pending text,
    // because flushing in that case would make the tree shape dependent on
    // where the flush points fall.
    flushCharacters();
  }
  FlushLoads();
  if (mOpSink) {
    bool hasOps = !mOpQueue.IsEmpty();
    if (hasOps) {
      mOpSink->MoveOpsFrom(mOpQueue);
    }
    return hasOps;
  }
  // no op sink: throw away ops
  mOpQueue.Clear();
  return false;
}

void
nsHtml5TreeBuilder::FlushLoads()
{
  if (MOZ_UNLIKELY(mBuilder)) {
    MOZ_ASSUME_UNREACHABLE("Must never flush loads with builder.");
    return;
  }
  if (!mSpeculativeLoadQueue.IsEmpty()) {
    mSpeculativeLoadStage->MoveSpeculativeLoadsFrom(mSpeculativeLoadQueue);
  }
}

void
nsHtml5TreeBuilder::SetDocumentCharset(nsACString& aCharset, 
                                       int32_t aCharsetSource)
{
  if (mBuilder) {
    mBuilder->SetDocumentCharsetAndSource(aCharset, aCharsetSource);
  } else if (mSpeculativeLoadStage) {
    mSpeculativeLoadQueue.AppendElement()->InitSetDocumentCharset(
      aCharset, aCharsetSource);
  } else {
    mOpQueue.AppendElement()->Init(
      eTreeOpSetDocumentCharset, aCharset, aCharsetSource);
  }
}

void
nsHtml5TreeBuilder::StreamEnded()
{
  MOZ_ASSERT(!mBuilder, "Must not call StreamEnded with builder.");
  MOZ_ASSERT(!fragment, "Must not parse fragments off the main thread.");
  nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
  NS_ASSERTION(treeOp, "Tree op allocation failed.");
  treeOp->Init(eTreeOpStreamEnded);
}

void
nsHtml5TreeBuilder::NeedsCharsetSwitchTo(const nsACString& aCharset,
                                         int32_t aCharsetSource,
                                         int32_t aLineNumber)
{
  if (MOZ_UNLIKELY(mBuilder)) {
    MOZ_ASSUME_UNREACHABLE("Must never switch charset with builder.");
    return;
  }
  nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
  NS_ASSERTION(treeOp, "Tree op allocation failed.");
  treeOp->Init(eTreeOpNeedsCharsetSwitchTo,
               aCharset,
               aCharsetSource,
               aLineNumber);
}

void
nsHtml5TreeBuilder::MaybeComplainAboutCharset(const char* aMsgId,
                                              bool aError,
                                              int32_t aLineNumber)
{
  if (MOZ_UNLIKELY(mBuilder)) {
    MOZ_ASSUME_UNREACHABLE("Must never complain about charset with builder.");
    return;
  }
  mOpQueue.AppendElement()->Init(aMsgId, aError, aLineNumber);
}

void
nsHtml5TreeBuilder::AddSnapshotToScript(nsAHtml5TreeBuilderState* aSnapshot, int32_t aLine)
{
  if (MOZ_UNLIKELY(mBuilder)) {
    MOZ_ASSUME_UNREACHABLE("Must never use snapshots with builder.");
    return;
  }
  NS_PRECONDITION(HasScript(), "No script to add a snapshot to!");
  NS_PRECONDITION(aSnapshot, "Got null snapshot.");
  mOpQueue.ElementAt(mOpQueue.Length() - 1).SetSnapshot(aSnapshot, aLine);
}

void
nsHtml5TreeBuilder::DropHandles()
{
  MOZ_ASSERT(!mBuilder, "Must not drop handles with builder.");
  mOldHandles.Clear();
  mHandlesUsed = 0;
}

void
nsHtml5TreeBuilder::MarkAsBroken()
{
  if (MOZ_UNLIKELY(mBuilder)) {
    MOZ_ASSUME_UNREACHABLE("Must not call this with builder.");
    return;
  }
  mOpQueue.Clear(); // Previous ops don't matter anymore
  mOpQueue.AppendElement()->Init(eTreeOpMarkAsBroken);
}

void
nsHtml5TreeBuilder::StartPlainTextViewSource(const nsAutoString& aTitle)
{
  MOZ_ASSERT(!mBuilder, "Must not view source with builder.");
  startTag(nsHtml5ElementName::ELT_TITLE,
           nsHtml5HtmlAttributes::EMPTY_ATTRIBUTES,
           false);

  // XUL will add the "Source of: " prefix.
  uint32_t length = aTitle.Length();
  if (length > INT32_MAX) {
    length = INT32_MAX;
  }
  characters(aTitle.get(), 0, (int32_t)length);
  endTag(nsHtml5ElementName::ELT_TITLE);

  startTag(nsHtml5ElementName::ELT_LINK,
           nsHtml5ViewSourceUtils::NewLinkAttributes(),
           false);

  startTag(nsHtml5ElementName::ELT_BODY,
           nsHtml5ViewSourceUtils::NewBodyAttributes(),
           false);

  StartPlainTextBody();
}

void
nsHtml5TreeBuilder::StartPlainText()
{
  MOZ_ASSERT(!mBuilder, "Must not view source with builder.");
  startTag(nsHtml5ElementName::ELT_LINK,
           nsHtml5PlainTextUtils::NewLinkAttributes(),
           false);

  StartPlainTextBody();
}

void
nsHtml5TreeBuilder::StartPlainTextBody()
{
  MOZ_ASSERT(!mBuilder, "Must not view source with builder.");
  startTag(nsHtml5ElementName::ELT_PRE,
           nsHtml5HtmlAttributes::EMPTY_ATTRIBUTES,
           false);
  needToDropLF = false;
}

// DocumentModeHandler
void
nsHtml5TreeBuilder::documentMode(nsHtml5DocumentMode m)
{
  if (mBuilder) {
    mBuilder->SetDocumentMode(m);
    return;
  }
  nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
  NS_ASSERTION(treeOp, "Tree op allocation failed.");
  treeOp->Init(m);
}

nsIContentHandle*
nsHtml5TreeBuilder::getDocumentFragmentForTemplate(nsIContentHandle* aTemplate)
{
  if (mBuilder) {
    return nsHtml5TreeOperation::GetDocumentFragmentForTemplate(static_cast<nsIContent*>(aTemplate));
  }
  nsHtml5TreeOperation* treeOp = mOpQueue.AppendElement();
  NS_ASSERTION(treeOp, "Tree op allocation failed.");
  nsIContentHandle* fragHandle = AllocateContentHandle();
  treeOp->Init(eTreeOpGetDocumentFragmentForTemplate, aTemplate, fragHandle);
  return fragHandle;
}

nsIContentHandle*
nsHtml5TreeBuilder::getFormPointerForContext(nsIContentHandle* aContext)
{
  MOZ_ASSERT(mBuilder, "Must have builder.");
  if (!aContext) {
    return nullptr;
  }

  MOZ_ASSERT(NS_IsMainThread());

  // aContext must always be an element that already exists
  // in the document.
  nsIContent* contextNode = static_cast<nsIContent*>(aContext);
  nsIContent* currentAncestor = contextNode;

  // We traverse the ancestors of the context node to find the nearest
  // form pointer. This traversal is why aContext must not be an emtpy handle.
  nsIContent* nearestForm = nullptr;
  while (currentAncestor) {
    if (currentAncestor->IsHTML(nsGkAtoms::form)) {
      nearestForm = currentAncestor;
      break;
    }
    currentAncestor = currentAncestor->GetParent();
  }

  if (!nearestForm) {
    return nullptr;
  }

  return nearestForm;
}

// Error reporting

void
nsHtml5TreeBuilder::EnableViewSource(nsHtml5Highlighter* aHighlighter)
{
  MOZ_ASSERT(!mBuilder, "Must not view source with builder.");
  mViewSource = aHighlighter;
}

void
nsHtml5TreeBuilder::errStrayStartTag(nsIAtom* aName)
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errStrayStartTag2", aName);
  }
}

void
nsHtml5TreeBuilder::errStrayEndTag(nsIAtom* aName)
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errStrayEndTag", aName);
  }
}

void
nsHtml5TreeBuilder::errUnclosedElements(int32_t aIndex, nsIAtom* aName)
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errUnclosedElements", aName);
  }
}

void
nsHtml5TreeBuilder::errUnclosedElementsImplied(int32_t aIndex, nsIAtom* aName)
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errUnclosedElementsImplied",
        aName);
  }
}

void
nsHtml5TreeBuilder::errUnclosedElementsCell(int32_t aIndex)
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errUnclosedElementsCell");
  }
}

void
nsHtml5TreeBuilder::errStrayDoctype()
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errStrayDoctype");
  }
}

void
nsHtml5TreeBuilder::errAlmostStandardsDoctype()
{
  if (MOZ_UNLIKELY(mViewSource) && !isSrcdocDocument) {
    mViewSource->AddErrorToCurrentRun("errAlmostStandardsDoctype");
  }
}

void
nsHtml5TreeBuilder::errQuirkyDoctype()
{
  if (MOZ_UNLIKELY(mViewSource) && !isSrcdocDocument) {
    mViewSource->AddErrorToCurrentRun("errQuirkyDoctype");
  }
}

void
nsHtml5TreeBuilder::errNonSpaceInTrailer()
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errNonSpaceInTrailer");
  }
}

void
nsHtml5TreeBuilder::errNonSpaceAfterFrameset()
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errNonSpaceAfterFrameset");
  }
}

void
nsHtml5TreeBuilder::errNonSpaceInFrameset()
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errNonSpaceInFrameset");
  }
}

void
nsHtml5TreeBuilder::errNonSpaceAfterBody()
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errNonSpaceAfterBody");
  }
}

void
nsHtml5TreeBuilder::errNonSpaceInColgroupInFragment()
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errNonSpaceInColgroupInFragment");
  }
}

void
nsHtml5TreeBuilder::errNonSpaceInNoscriptInHead()
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errNonSpaceInNoscriptInHead");
  }
}

void
nsHtml5TreeBuilder::errFooBetweenHeadAndBody(nsIAtom* aName)
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errFooBetweenHeadAndBody", aName);
  }
}

void
nsHtml5TreeBuilder::errStartTagWithoutDoctype()
{
  if (MOZ_UNLIKELY(mViewSource) && !isSrcdocDocument) {
    mViewSource->AddErrorToCurrentRun("errStartTagWithoutDoctype");
  }
}

void
nsHtml5TreeBuilder::errNoSelectInTableScope()
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errNoSelectInTableScope");
  }
}

void
nsHtml5TreeBuilder::errStartSelectWhereEndSelectExpected()
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun(
        "errStartSelectWhereEndSelectExpected");
  }
}

void
nsHtml5TreeBuilder::errStartTagWithSelectOpen(nsIAtom* aName)
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errStartTagWithSelectOpen", aName);
  }
}

void
nsHtml5TreeBuilder::errBadStartTagInHead(nsIAtom* aName)
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errBadStartTagInHead2", aName);
  }
}

void
nsHtml5TreeBuilder::errImage()
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errImage");
  }
}

void
nsHtml5TreeBuilder::errIsindex()
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errIsindex");
  }
}

void
nsHtml5TreeBuilder::errFooSeenWhenFooOpen(nsIAtom* aName)
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errFooSeenWhenFooOpen", aName);
  }
}

void
nsHtml5TreeBuilder::errHeadingWhenHeadingOpen()
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errHeadingWhenHeadingOpen");
  }
}

void
nsHtml5TreeBuilder::errFramesetStart()
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errFramesetStart");
  }
}

void
nsHtml5TreeBuilder::errNoCellToClose()
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errNoCellToClose");
  }
}

void
nsHtml5TreeBuilder::errStartTagInTable(nsIAtom* aName)
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errStartTagInTable", aName);
  }
}

void
nsHtml5TreeBuilder::errFormWhenFormOpen()
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errFormWhenFormOpen");
  }
}

void
nsHtml5TreeBuilder::errTableSeenWhileTableOpen()
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errTableSeenWhileTableOpen");
  }
}

void
nsHtml5TreeBuilder::errStartTagInTableBody(nsIAtom* aName)
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errStartTagInTableBody", aName);
  }
}

void
nsHtml5TreeBuilder::errEndTagSeenWithoutDoctype()
{
  if (MOZ_UNLIKELY(mViewSource) && !isSrcdocDocument) {
    mViewSource->AddErrorToCurrentRun("errEndTagSeenWithoutDoctype");
  }
}

void
nsHtml5TreeBuilder::errEndTagAfterBody()
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errEndTagAfterBody");
  }
}

void
nsHtml5TreeBuilder::errEndTagSeenWithSelectOpen(nsIAtom* aName)
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errEndTagSeenWithSelectOpen",
        aName);
  }
}

void
nsHtml5TreeBuilder::errGarbageInColgroup()
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errGarbageInColgroup");
  }
}

void
nsHtml5TreeBuilder::errEndTagBr()
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errEndTagBr");
  }
}

void
nsHtml5TreeBuilder::errNoElementToCloseButEndTagSeen(nsIAtom* aName)
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun(
        "errNoElementToCloseButEndTagSeen", aName);
  }
}

void
nsHtml5TreeBuilder::errHtmlStartTagInForeignContext(nsIAtom* aName)
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errHtmlStartTagInForeignContext",
        aName);
  }
}

void
nsHtml5TreeBuilder::errTableClosedWhileCaptionOpen()
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errTableClosedWhileCaptionOpen");
  }
}

void
nsHtml5TreeBuilder::errNoTableRowToClose()
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errNoTableRowToClose");
  }
}

void
nsHtml5TreeBuilder::errNonSpaceInTable()
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errNonSpaceInTable");
  }
}

void
nsHtml5TreeBuilder::errUnclosedChildrenInRuby()
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errUnclosedChildrenInRuby");
  }
}

void
nsHtml5TreeBuilder::errStartTagSeenWithoutRuby(nsIAtom* aName)
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errStartTagSeenWithoutRuby",
        aName);
  }
}

void
nsHtml5TreeBuilder::errSelfClosing()
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentSlash("errSelfClosing");
  }
}

void
nsHtml5TreeBuilder::errNoCheckUnclosedElementsOnStack()
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun(
        "errNoCheckUnclosedElementsOnStack");
  }
}

void
nsHtml5TreeBuilder::errEndTagDidNotMatchCurrentOpenElement(nsIAtom* aName,
                                                           nsIAtom* aOther)
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun(
        "errEndTagDidNotMatchCurrentOpenElement", aName, aOther);
  }
}

void
nsHtml5TreeBuilder::errEndTagViolatesNestingRules(nsIAtom* aName)
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errEndTagViolatesNestingRules", aName);
  }
}

void
nsHtml5TreeBuilder::errEndWithUnclosedElements(nsIAtom* aName)
{
  if (MOZ_UNLIKELY(mViewSource)) {
    mViewSource->AddErrorToCurrentRun("errEndWithUnclosedElements", aName);
  }
}
