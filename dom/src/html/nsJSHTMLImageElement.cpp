/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "NPL"); you may not use this file except in
 * compliance with the NPL.  You may obtain a copy of the NPL at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the NPL is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
 * for the specific language governing rights and limitations under the
 * NPL.
 *
 * The Initial Developer of this code under the NPL is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation.  All Rights
 * Reserved.
 */
/* AUTO-GENERATED. DO NOT EDIT!!! */

#include "jsapi.h"
#include "nscore.h"
#include "nsIScriptContext.h"
#include "nsIJSScriptObject.h"
#include "nsIScriptObjectOwner.h"
#include "nsIScriptGlobalObject.h"
#include "nsIPtr.h"
#include "nsString.h"
#include "nsIDOMImage.h"
#include "nsIDOMHTMLImageElement.h"
#include "nsIDOMNativeObjectRegistry.h"
#include "nsIServiceManager.h"
#include "nsRepository.h"
#include "nsDOMCID.h"


static NS_DEFINE_IID(kIScriptObjectOwnerIID, NS_ISCRIPTOBJECTOWNER_IID);
static NS_DEFINE_IID(kIJSScriptObjectIID, NS_IJSSCRIPTOBJECT_IID);
static NS_DEFINE_IID(kIScriptGlobalObjectIID, NS_ISCRIPTGLOBALOBJECT_IID);
static NS_DEFINE_IID(kIImageIID, NS_IDOMIMAGE_IID);
static NS_DEFINE_IID(kIHTMLImageElementIID, NS_IDOMHTMLIMAGEELEMENT_IID);

NS_DEF_PTR(nsIDOMImage);
NS_DEF_PTR(nsIDOMHTMLImageElement);

//
// HTMLImageElement property ids
//
enum HTMLImageElement_slots {
  HTMLIMAGEELEMENT_LOWSRC = -11,
  HTMLIMAGEELEMENT_NAME = -12,
  HTMLIMAGEELEMENT_ALIGN = -13,
  HTMLIMAGEELEMENT_ALT = -14,
  HTMLIMAGEELEMENT_BORDER = -15,
  HTMLIMAGEELEMENT_HEIGHT = -16,
  HTMLIMAGEELEMENT_HSPACE = -17,
  HTMLIMAGEELEMENT_ISMAP = -18,
  HTMLIMAGEELEMENT_LONGDESC = -19,
  HTMLIMAGEELEMENT_SRC = -110,
  HTMLIMAGEELEMENT_USEMAP = -111,
  HTMLIMAGEELEMENT_VSPACE = -112,
  HTMLIMAGEELEMENT_WIDTH = -113
};

/***********************************************************************/
//
// HTMLImageElement Properties Getter
//
PR_STATIC_CALLBACK(JSBool)
GetHTMLImageElementProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  nsIDOMHTMLImageElement *a = (nsIDOMHTMLImageElement*)JS_GetPrivate(cx, obj);

  // If there's no private data, this must be the prototype, so ignore
  if (nsnull == a) {
    return JS_TRUE;
  }

  if (JSVAL_IS_INT(id)) {
    switch(JSVAL_TO_INT(id)) {
      case HTMLIMAGEELEMENT_LOWSRC:
      {
        nsAutoString prop;
        if (NS_OK == a->GetLowSrc(prop)) {
          JSString *jsstring = JS_NewUCStringCopyN(cx, prop, prop.Length());
          // set the return value
          *vp = STRING_TO_JSVAL(jsstring);
        }
        else {
          return JS_FALSE;
        }
        break;
      }
      case HTMLIMAGEELEMENT_NAME:
      {
        nsAutoString prop;
        if (NS_OK == a->GetName(prop)) {
          JSString *jsstring = JS_NewUCStringCopyN(cx, prop, prop.Length());
          // set the return value
          *vp = STRING_TO_JSVAL(jsstring);
        }
        else {
          return JS_FALSE;
        }
        break;
      }
      case HTMLIMAGEELEMENT_ALIGN:
      {
        nsAutoString prop;
        if (NS_OK == a->GetAlign(prop)) {
          JSString *jsstring = JS_NewUCStringCopyN(cx, prop, prop.Length());
          // set the return value
          *vp = STRING_TO_JSVAL(jsstring);
        }
        else {
          return JS_FALSE;
        }
        break;
      }
      case HTMLIMAGEELEMENT_ALT:
      {
        nsAutoString prop;
        if (NS_OK == a->GetAlt(prop)) {
          JSString *jsstring = JS_NewUCStringCopyN(cx, prop, prop.Length());
          // set the return value
          *vp = STRING_TO_JSVAL(jsstring);
        }
        else {
          return JS_FALSE;
        }
        break;
      }
      case HTMLIMAGEELEMENT_BORDER:
      {
        nsAutoString prop;
        if (NS_OK == a->GetBorder(prop)) {
          JSString *jsstring = JS_NewUCStringCopyN(cx, prop, prop.Length());
          // set the return value
          *vp = STRING_TO_JSVAL(jsstring);
        }
        else {
          return JS_FALSE;
        }
        break;
      }
      case HTMLIMAGEELEMENT_HEIGHT:
      {
        nsAutoString prop;
        if (NS_OK == a->GetHeight(prop)) {
          JSString *jsstring = JS_NewUCStringCopyN(cx, prop, prop.Length());
          // set the return value
          *vp = STRING_TO_JSVAL(jsstring);
        }
        else {
          return JS_FALSE;
        }
        break;
      }
      case HTMLIMAGEELEMENT_HSPACE:
      {
        nsAutoString prop;
        if (NS_OK == a->GetHspace(prop)) {
          JSString *jsstring = JS_NewUCStringCopyN(cx, prop, prop.Length());
          // set the return value
          *vp = STRING_TO_JSVAL(jsstring);
        }
        else {
          return JS_FALSE;
        }
        break;
      }
      case HTMLIMAGEELEMENT_ISMAP:
      {
        PRBool prop;
        if (NS_OK == a->GetIsMap(&prop)) {
          *vp = BOOLEAN_TO_JSVAL(prop);
        }
        else {
          return JS_FALSE;
        }
        break;
      }
      case HTMLIMAGEELEMENT_LONGDESC:
      {
        nsAutoString prop;
        if (NS_OK == a->GetLongDesc(prop)) {
          JSString *jsstring = JS_NewUCStringCopyN(cx, prop, prop.Length());
          // set the return value
          *vp = STRING_TO_JSVAL(jsstring);
        }
        else {
          return JS_FALSE;
        }
        break;
      }
      case HTMLIMAGEELEMENT_SRC:
      {
        nsAutoString prop;
        if (NS_OK == a->GetSrc(prop)) {
          JSString *jsstring = JS_NewUCStringCopyN(cx, prop, prop.Length());
          // set the return value
          *vp = STRING_TO_JSVAL(jsstring);
        }
        else {
          return JS_FALSE;
        }
        break;
      }
      case HTMLIMAGEELEMENT_USEMAP:
      {
        nsAutoString prop;
        if (NS_OK == a->GetUseMap(prop)) {
          JSString *jsstring = JS_NewUCStringCopyN(cx, prop, prop.Length());
          // set the return value
          *vp = STRING_TO_JSVAL(jsstring);
        }
        else {
          return JS_FALSE;
        }
        break;
      }
      case HTMLIMAGEELEMENT_VSPACE:
      {
        nsAutoString prop;
        if (NS_OK == a->GetVspace(prop)) {
          JSString *jsstring = JS_NewUCStringCopyN(cx, prop, prop.Length());
          // set the return value
          *vp = STRING_TO_JSVAL(jsstring);
        }
        else {
          return JS_FALSE;
        }
        break;
      }
      case HTMLIMAGEELEMENT_WIDTH:
      {
        nsAutoString prop;
        if (NS_OK == a->GetWidth(prop)) {
          JSString *jsstring = JS_NewUCStringCopyN(cx, prop, prop.Length());
          // set the return value
          *vp = STRING_TO_JSVAL(jsstring);
        }
        else {
          return JS_FALSE;
        }
        break;
      }
      default:
      {
        nsIJSScriptObject *object;
        if (NS_OK == a->QueryInterface(kIJSScriptObjectIID, (void**)&object)) {
          PRBool rval;
          rval =  object->GetProperty(cx, id, vp);
          NS_RELEASE(object);
          return rval;
        }
      }
    }
  }
  else {
    nsIJSScriptObject *object;
    if (NS_OK == a->QueryInterface(kIJSScriptObjectIID, (void**)&object)) {
      PRBool rval;
      rval =  object->GetProperty(cx, id, vp);
      NS_RELEASE(object);
      return rval;
    }
  }

  return PR_TRUE;
}

/***********************************************************************/
//
// HTMLImageElement Properties Setter
//
PR_STATIC_CALLBACK(JSBool)
SetHTMLImageElementProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
  nsIDOMHTMLImageElement *a = (nsIDOMHTMLImageElement*)JS_GetPrivate(cx, obj);

  // If there's no private data, this must be the prototype, so ignore
  if (nsnull == a) {
    return JS_TRUE;
  }

  if (JSVAL_IS_INT(id)) {
    switch(JSVAL_TO_INT(id)) {
      case HTMLIMAGEELEMENT_LOWSRC:
      {
        nsAutoString prop;
        JSString *jsstring;
        if ((jsstring = JS_ValueToString(cx, *vp)) != nsnull) {
          prop.SetString(JS_GetStringChars(jsstring));
        }
        else {
          prop.SetString((const char *)nsnull);
        }
      
        a->SetLowSrc(prop);
        
        break;
      }
      case HTMLIMAGEELEMENT_NAME:
      {
        nsAutoString prop;
        JSString *jsstring;
        if ((jsstring = JS_ValueToString(cx, *vp)) != nsnull) {
          prop.SetString(JS_GetStringChars(jsstring));
        }
        else {
          prop.SetString((const char *)nsnull);
        }
      
        a->SetName(prop);
        
        break;
      }
      case HTMLIMAGEELEMENT_ALIGN:
      {
        nsAutoString prop;
        JSString *jsstring;
        if ((jsstring = JS_ValueToString(cx, *vp)) != nsnull) {
          prop.SetString(JS_GetStringChars(jsstring));
        }
        else {
          prop.SetString((const char *)nsnull);
        }
      
        a->SetAlign(prop);
        
        break;
      }
      case HTMLIMAGEELEMENT_ALT:
      {
        nsAutoString prop;
        JSString *jsstring;
        if ((jsstring = JS_ValueToString(cx, *vp)) != nsnull) {
          prop.SetString(JS_GetStringChars(jsstring));
        }
        else {
          prop.SetString((const char *)nsnull);
        }
      
        a->SetAlt(prop);
        
        break;
      }
      case HTMLIMAGEELEMENT_BORDER:
      {
        nsAutoString prop;
        JSString *jsstring;
        if ((jsstring = JS_ValueToString(cx, *vp)) != nsnull) {
          prop.SetString(JS_GetStringChars(jsstring));
        }
        else {
          prop.SetString((const char *)nsnull);
        }
      
        a->SetBorder(prop);
        
        break;
      }
      case HTMLIMAGEELEMENT_HEIGHT:
      {
        nsAutoString prop;
        JSString *jsstring;
        if ((jsstring = JS_ValueToString(cx, *vp)) != nsnull) {
          prop.SetString(JS_GetStringChars(jsstring));
        }
        else {
          prop.SetString((const char *)nsnull);
        }
      
        a->SetHeight(prop);
        
        break;
      }
      case HTMLIMAGEELEMENT_HSPACE:
      {
        nsAutoString prop;
        JSString *jsstring;
        if ((jsstring = JS_ValueToString(cx, *vp)) != nsnull) {
          prop.SetString(JS_GetStringChars(jsstring));
        }
        else {
          prop.SetString((const char *)nsnull);
        }
      
        a->SetHspace(prop);
        
        break;
      }
      case HTMLIMAGEELEMENT_ISMAP:
      {
        PRBool prop;
        JSBool temp;
        if (JSVAL_IS_BOOLEAN(*vp) && JS_ValueToBoolean(cx, *vp, &temp)) {
          prop = (PRBool)temp;
        }
        else {
          JS_ReportError(cx, "Parameter must be a boolean");
          return JS_FALSE;
        }
      
        a->SetIsMap(prop);
        
        break;
      }
      case HTMLIMAGEELEMENT_LONGDESC:
      {
        nsAutoString prop;
        JSString *jsstring;
        if ((jsstring = JS_ValueToString(cx, *vp)) != nsnull) {
          prop.SetString(JS_GetStringChars(jsstring));
        }
        else {
          prop.SetString((const char *)nsnull);
        }
      
        a->SetLongDesc(prop);
        
        break;
      }
      case HTMLIMAGEELEMENT_SRC:
      {
        nsAutoString prop;
        JSString *jsstring;
        if ((jsstring = JS_ValueToString(cx, *vp)) != nsnull) {
          prop.SetString(JS_GetStringChars(jsstring));
        }
        else {
          prop.SetString((const char *)nsnull);
        }
      
        a->SetSrc(prop);
        
        break;
      }
      case HTMLIMAGEELEMENT_USEMAP:
      {
        nsAutoString prop;
        JSString *jsstring;
        if ((jsstring = JS_ValueToString(cx, *vp)) != nsnull) {
          prop.SetString(JS_GetStringChars(jsstring));
        }
        else {
          prop.SetString((const char *)nsnull);
        }
      
        a->SetUseMap(prop);
        
        break;
      }
      case HTMLIMAGEELEMENT_VSPACE:
      {
        nsAutoString prop;
        JSString *jsstring;
        if ((jsstring = JS_ValueToString(cx, *vp)) != nsnull) {
          prop.SetString(JS_GetStringChars(jsstring));
        }
        else {
          prop.SetString((const char *)nsnull);
        }
      
        a->SetVspace(prop);
        
        break;
      }
      case HTMLIMAGEELEMENT_WIDTH:
      {
        nsAutoString prop;
        JSString *jsstring;
        if ((jsstring = JS_ValueToString(cx, *vp)) != nsnull) {
          prop.SetString(JS_GetStringChars(jsstring));
        }
        else {
          prop.SetString((const char *)nsnull);
        }
      
        a->SetWidth(prop);
        
        break;
      }
      default:
      {
        nsIJSScriptObject *object;
        if (NS_OK == a->QueryInterface(kIJSScriptObjectIID, (void**)&object)) {
          PRBool rval;
          rval =  object->SetProperty(cx, id, vp);
          NS_RELEASE(object);
          return rval;
        }
      }
    }
  }
  else {
    nsIJSScriptObject *object;
    if (NS_OK == a->QueryInterface(kIJSScriptObjectIID, (void**)&object)) {
      PRBool rval;
      rval =  object->SetProperty(cx, id, vp);
      NS_RELEASE(object);
      return rval;
    }
  }

  return PR_TRUE;
}


//
// HTMLImageElement finalizer
//
PR_STATIC_CALLBACK(void)
FinalizeHTMLImageElement(JSContext *cx, JSObject *obj)
{
  nsIDOMHTMLImageElement *a = (nsIDOMHTMLImageElement*)JS_GetPrivate(cx, obj);
  
  if (nsnull != a) {
    // get the js object
    nsIScriptObjectOwner *owner = nsnull;
    if (NS_OK == a->QueryInterface(kIScriptObjectOwnerIID, (void**)&owner)) {
      owner->SetScriptObject(nsnull);
      NS_RELEASE(owner);
    }

    NS_RELEASE(a);
  }
}


//
// HTMLImageElement enumerate
//
PR_STATIC_CALLBACK(JSBool)
EnumerateHTMLImageElement(JSContext *cx, JSObject *obj)
{
  nsIDOMHTMLImageElement *a = (nsIDOMHTMLImageElement*)JS_GetPrivate(cx, obj);
  
  if (nsnull != a) {
    // get the js object
    nsIJSScriptObject *object;
    if (NS_OK == a->QueryInterface(kIJSScriptObjectIID, (void**)&object)) {
      object->EnumerateProperty(cx);
      NS_RELEASE(object);
    }
  }
  return JS_TRUE;
}


//
// HTMLImageElement resolve
//
PR_STATIC_CALLBACK(JSBool)
ResolveHTMLImageElement(JSContext *cx, JSObject *obj, jsval id)
{
  nsIDOMHTMLImageElement *a = (nsIDOMHTMLImageElement*)JS_GetPrivate(cx, obj);
  
  if (nsnull != a) {
    // get the js object
    nsIJSScriptObject *object;
    if (NS_OK == a->QueryInterface(kIJSScriptObjectIID, (void**)&object)) {
      object->Resolve(cx, id);
      NS_RELEASE(object);
    }
  }
  return JS_TRUE;
}


/***********************************************************************/
//
// class for HTMLImageElement
//
JSClass HTMLImageElementClass = {
  "HTMLImageElement", 
  JSCLASS_HAS_PRIVATE,
  JS_PropertyStub,
  JS_PropertyStub,
  GetHTMLImageElementProperty,
  SetHTMLImageElementProperty,
  EnumerateHTMLImageElement,
  ResolveHTMLImageElement,
  JS_ConvertStub,
  FinalizeHTMLImageElement
};


//
// HTMLImageElement class properties
//
static JSPropertySpec HTMLImageElementProperties[] =
{
  {"lowSrc",    HTMLIMAGEELEMENT_LOWSRC,    JSPROP_ENUMERATE},
  {"name",    HTMLIMAGEELEMENT_NAME,    JSPROP_ENUMERATE},
  {"align",    HTMLIMAGEELEMENT_ALIGN,    JSPROP_ENUMERATE},
  {"alt",    HTMLIMAGEELEMENT_ALT,    JSPROP_ENUMERATE},
  {"border",    HTMLIMAGEELEMENT_BORDER,    JSPROP_ENUMERATE},
  {"height",    HTMLIMAGEELEMENT_HEIGHT,    JSPROP_ENUMERATE},
  {"hspace",    HTMLIMAGEELEMENT_HSPACE,    JSPROP_ENUMERATE},
  {"isMap",    HTMLIMAGEELEMENT_ISMAP,    JSPROP_ENUMERATE},
  {"longDesc",    HTMLIMAGEELEMENT_LONGDESC,    JSPROP_ENUMERATE},
  {"src",    HTMLIMAGEELEMENT_SRC,    JSPROP_ENUMERATE},
  {"useMap",    HTMLIMAGEELEMENT_USEMAP,    JSPROP_ENUMERATE},
  {"vspace",    HTMLIMAGEELEMENT_VSPACE,    JSPROP_ENUMERATE},
  {"width",    HTMLIMAGEELEMENT_WIDTH,    JSPROP_ENUMERATE},
  {0}
};


//
// HTMLImageElement class methods
//
static JSFunctionSpec HTMLImageElementMethods[] = 
{
  {0}
};


//
// HTMLImageElement constructor
//
PR_STATIC_CALLBACK(JSBool)
HTMLImageElement(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
  nsresult result;
  nsIID factoryCID;
  nsIDOMNativeObjectRegistry *registry;
  nsIDOMHTMLImageElementFactory *factory;
  nsIDOMHTMLImageElement *nativeThis;
  nsIScriptObjectOwner *owner = nsnull;

  static NS_DEFINE_IID(kDOMNativeObjectRegistryCID, NS_DOM_NATIVE_OBJECT_REGISTRY_CID);
  static NS_DEFINE_IID(kIDOMNativeObjectRegistryIID, NS_IDOM_NATIVE_OBJECT_REGISTRY_IID);
  static NS_DEFINE_IID(kIDOMHTMLImageElementFactoryIID, NS_IDOMHTMLIMAGEELEMENTFACTORY_IID);

  result = nsServiceManager::GetService(kDOMNativeObjectRegistryCID,
                                        kIDOMNativeObjectRegistryIID,
                                        (nsISupports **)&registry);
  if (NS_OK != result) {
    return JS_FALSE;
  }

  result = registry->GetFactoryCID("HTMLImageElement", factoryCID);
  nsServiceManager::ReleaseService(kDOMNativeObjectRegistryCID,
                                   registry);
  if (NS_OK != result) {
    return JS_FALSE;
  }

  result = nsRepository::CreateInstance(factoryCID,
                                        nsnull,
                                        kIDOMHTMLImageElementFactoryIID,
                                        (void **)&factory);
  if (NS_OK != result) {
    return JS_FALSE;
  }

  result = factory->CreateInstance(&nativeThis);
  NS_RELEASE(factory);
  if (NS_OK != result) {
    return JS_FALSE;
  }

  result = nativeThis->QueryInterface(kIScriptObjectOwnerIID, (void **)&owner);
  if (NS_OK != result) {
    NS_RELEASE(nativeThis);
    return JS_FALSE;
  }

  owner->SetScriptObject((void *)obj);
  JS_SetPrivate(cx, obj, nativeThis);

  NS_RELEASE(owner);
  return JS_TRUE;
}

//
// HTMLImageElement class initialization
//
nsresult NS_InitHTMLImageElementClass(nsIScriptContext *aContext, void **aPrototype)
{
  JSContext *jscontext = (JSContext *)aContext->GetNativeContext();
  JSObject *proto = nsnull;
  JSObject *constructor = nsnull;
  JSObject *parent_proto = nsnull;
  JSObject *global = JS_GetGlobalObject(jscontext);
  jsval vp;

  if ((PR_TRUE != JS_LookupProperty(jscontext, global, "HTMLImageElement", &vp)) ||
      !JSVAL_IS_OBJECT(vp) ||
      ((constructor = JSVAL_TO_OBJECT(vp)) == nsnull) ||
      (PR_TRUE != JS_LookupProperty(jscontext, JSVAL_TO_OBJECT(vp), "prototype", &vp)) || 
      !JSVAL_IS_OBJECT(vp)) {

    if (NS_OK != NS_InitHTMLElementClass(aContext, (void **)&parent_proto)) {
      return NS_ERROR_FAILURE;
    }
    proto = JS_InitClass(jscontext,     // context
                         global,        // global object
                         parent_proto,  // parent proto 
                         &HTMLImageElementClass,      // JSClass
                         HTMLImageElement,            // JSNative ctor
                         0,             // ctor args
                         HTMLImageElementProperties,  // proto props
                         HTMLImageElementMethods,     // proto funcs
                         nsnull,        // ctor props (static)
                         nsnull);       // ctor funcs (static)
    if (nsnull == proto) {
      return NS_ERROR_FAILURE;
    }

    JS_AliasProperty(jscontext, global, "HTMLImageElement", "Image");
  }
  else if ((nsnull != constructor) && JSVAL_IS_OBJECT(vp)) {
    proto = JSVAL_TO_OBJECT(vp);
  }
  else {
    return NS_ERROR_FAILURE;
  }

  if (aPrototype) {
    *aPrototype = proto;
  }
  return NS_OK;
}


//
// Method for creating a new HTMLImageElement JavaScript object
//
extern "C" NS_DOM nsresult NS_NewScriptHTMLImageElement(nsIScriptContext *aContext, nsISupports *aSupports, nsISupports *aParent, void **aReturn)
{
  NS_PRECONDITION(nsnull != aContext && nsnull != aSupports && nsnull != aReturn, "null argument to NS_NewScriptHTMLImageElement");
  JSObject *proto;
  JSObject *parent;
  nsIScriptObjectOwner *owner;
  JSContext *jscontext = (JSContext *)aContext->GetNativeContext();
  nsresult result = NS_OK;
  nsIDOMHTMLImageElement *aHTMLImageElement;

  if (nsnull == aParent) {
    parent = nsnull;
  }
  else if (NS_OK == aParent->QueryInterface(kIScriptObjectOwnerIID, (void**)&owner)) {
    if (NS_OK != owner->GetScriptObject(aContext, (void **)&parent)) {
      NS_RELEASE(owner);
      return NS_ERROR_FAILURE;
    }
    NS_RELEASE(owner);
  }
  else {
    return NS_ERROR_FAILURE;
  }

  if (NS_OK != NS_InitHTMLImageElementClass(aContext, (void **)&proto)) {
    return NS_ERROR_FAILURE;
  }

  result = aSupports->QueryInterface(kIHTMLImageElementIID, (void **)&aHTMLImageElement);
  if (NS_OK != result) {
    return result;
  }

  // create a js object for this class
  *aReturn = JS_NewObject(jscontext, &HTMLImageElementClass, proto, parent);
  if (nsnull != *aReturn) {
    // connect the native object to the js object
    JS_SetPrivate(jscontext, (JSObject *)*aReturn, aHTMLImageElement);
  }
  else {
    NS_RELEASE(aHTMLImageElement);
    return NS_ERROR_FAILURE; 
  }

  return NS_OK;
}
