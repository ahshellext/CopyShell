// CopyMenu.cpp : CCopyMenu 的实现

#include "stdafx.h"
#include "CopyMenu.h"

#include <atlconv.h>
#include <string>

#define INT2STR(_num) std::to_string((long long) _num)

// CCopyMenu

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

HRESULT STDMETHODCALLTYPE CCopyMenu::Initialize( 
	/* [annotation][unique][in] */ 
	__in_opt  PCIDLIST_ABSOLUTE pidlFolder,
	/* [annotation][unique][in] */ 
	__in_opt  IDataObject *pdtobj,
	/* [annotation][unique][in] */ 
	__in_opt  HKEY hkeyProgID)
{
	/*
	SHSTDAPI_(UINT) DragQueryFileW(
		__in HDROP hDrop,
		__in UINT iFile, 
		__out_ecount_opt(cch) LPWSTR lpszFile,
		__in UINT cch);
	*/
	HRESULT hr = E_INVALIDARG;
	
	FORMATETC fe = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	STGMEDIUM stm = {};
	if (SUCCEEDED(pdtobj->GetData(&fe, &stm))) {
		HDROP hDrop = static_cast<HDROP>(GlobalLock(stm.hGlobal));
		if (hDrop != NULL) {
			UINT cnt = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
			if (cnt != 0) {

				sel_files.clear();
				for (UINT i = 0; i < cnt; i++) {

					LPWSTR str = new WCHAR[2048];
					memset(str, 0, 2048);
					int size = DragQueryFile(hDrop, 
						i, str, 2048);
					if (size == 0) continue;

					std::string name = ATL::CW2A(str);
					delete []str;

					if (name == "") continue;
					sel_files.push_back(name);

				}
				hr = S_OK;
				GlobalUnlock(stm.hGlobal);
			}
			ReleaseStgMedium(&stm);
		}
	}
	return hr;
}

const UINT MENU_ID_1 = 0;
const UINT MENU_ID_2 = 1;
const UINT MENU_ID_3 = 2;
const UINT MENU_ID_4 = 3;

HRESULT STDMETHODCALLTYPE CCopyMenu::QueryContextMenu( 
	/* [annotation][in] */ 
	__in  HMENU hmenu,
	/* [annotation][in] */ 
	__in  UINT indexMenu,
	/* [annotation][in] */ 
	__in  UINT idCmdFirst,
	/* [annotation][in] */ 
	__in  UINT idCmdLast,
	/* [annotation][in] */ 
	__in  UINT uFlags)
{
	if (uFlags & CMF_DEFAULTONLY)
		return MAKE_SCODE(SEVERITY_SUCCESS, FACILITY_NULL, 0);

	HMENU subMenu = CreateMenu();

	MENUITEMINFO outMenu;

	outMenu.cbSize = sizeof(MENUITEMINFO);
	outMenu.fMask = MIIM_STRING | MIIM_SUBMENU;
	outMenu.dwTypeData = _T("パスのコピー");
	outMenu.hSubMenu = subMenu;

	MENUITEMINFO cpy_menuItem;
	MENUITEMINFO cpy_qte_menuItem;
	MENUITEMINFO cpy_dbl_menuItem;
	MENUITEMINFO cpy_qte_dbl_menuItem;

	cpy_menuItem.cbSize = sizeof(MENUITEMINFO);
	cpy_menuItem.fMask = MIIM_STRING | MIIM_ID;
	cpy_menuItem.dwTypeData = _T("引用記号抜き");
	cpy_menuItem.wID = idCmdFirst + MENU_ID_1;

	cpy_qte_menuItem.cbSize = sizeof(MENUITEMINFO);
	cpy_qte_menuItem.fMask = MIIM_STRING | MIIM_ID;
	cpy_qte_menuItem.dwTypeData = _T("引用記号付き");
	cpy_qte_menuItem.wID = idCmdFirst + MENU_ID_2;

	cpy_dbl_menuItem.cbSize = sizeof(MENUITEMINFO);
	cpy_dbl_menuItem.fMask = MIIM_STRING | MIIM_ID;
	cpy_dbl_menuItem.dwTypeData = _T("引用記号抜き ダブルバックスラッシュ");
	cpy_dbl_menuItem.wID = idCmdFirst + MENU_ID_3;

	cpy_qte_dbl_menuItem.cbSize = sizeof(MENUITEMINFO);
	cpy_qte_dbl_menuItem.fMask = MIIM_STRING | MIIM_ID;
	cpy_qte_dbl_menuItem.dwTypeData = _T("引用記号付き ダブルバックスラッシュ");
	cpy_qte_dbl_menuItem.wID = idCmdFirst + MENU_ID_4;

	/*
	InsertMenuItemW(
	__in HMENU hmenu,
	__in UINT item,
	__in BOOL fByPosition,
	__in LPCMENUITEMINFOW lpmi);

	InsertMenuW(
	__in HMENU hMenu,
	__in UINT uPosition,
	__in UINT uFlags,
	__in UINT_PTR uIDNewItem,
	__in_opt LPCWSTR lpNewItem);
	*/

	InsertMenuItem(hmenu, indexMenu, true, &outMenu);

	InsertMenuItem(subMenu, 0, true, &cpy_menuItem);
	InsertMenuItem(subMenu, 1, true, &cpy_qte_menuItem);
	InsertMenuItem(subMenu, 2, true, &cpy_dbl_menuItem);
	InsertMenuItem(subMenu, 3, true, &cpy_qte_dbl_menuItem);

	return MAKE_SCODE(SEVERITY_SUCCESS, FACILITY_NULL, 4);
}

HRESULT STDMETHODCALLTYPE CCopyMenu::InvokeCommand( 
	/* [annotation][in] */ 
	__in  CMINVOKECOMMANDINFO *pici)
{
	UINT idCmd = LOWORD(pici->lpVerb);
	if (HIWORD(pici->lpVerb) != 0)
		return E_INVALIDARG;

	/*
	MessageBoxW(
		__in_opt HWND hWnd,
		__in_opt LPCWSTR lpText,
		__in_opt LPCWSTR lpCaption,
		__in UINT uType);
	*/
	switch (idCmd) {
	case MENU_ID_1: // 引用記号抜き
		MessageBox(NULL, s2ws(INT2STR(sel_files.size())).c_str(), _T("CopyShell"), MB_OK);
		break;
	case MENU_ID_2: // 引用記号付き
		MessageBox(NULL, _T("MENU_ID_02"), _T("CopyShell"), MB_OK);
		break;
	case MENU_ID_3: // 引用記号抜き ダブルバックスラッシュ
		MessageBox(NULL, _T("MENU_ID_03"), _T("CopyShell"), MB_OK);
		break;
	case MENU_ID_4: // 引用記号付き ダブルバックスラッシュ
		MessageBox(NULL, _T("MENU_ID_04"), _T("CopyShell"), MB_OK);
		break;
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CCopyMenu::GetCommandString( 
	/* [annotation][in] */ 
	__in  UINT_PTR idCmd,
	/* [annotation][in] */ 
	__in  UINT uType,
	/* [annotation][in] */ 
	__reserved  UINT *pReserved,
	/* [annotation][out] */ 
	__out_awcount(!(uType & GCS_UNICODE), cchMax)  LPSTR pszName,
	/* [annotation][in] */ 
	__in  UINT cchMax)
{
	return S_OK;
}