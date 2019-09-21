// CopyMenu.cpp : CCopyMenu 的实现

#include "stdafx.h"
#include "CopyMenu.h"


// CCopyMenu

HRESULT STDMETHODCALLTYPE CCopyMenu::Initialize( 
	/* [annotation][unique][in] */ 
	__in_opt  PCIDLIST_ABSOLUTE pidlFolder,
	/* [annotation][unique][in] */ 
	__in_opt  IDataObject *pdtobj,
	/* [annotation][unique][in] */ 
	__in_opt  HKEY hkeyProgID)
{
	return S_OK;
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

	switch (idCmd) {
	case MENU_ID_1:
		MessageBox(NULL, _T("MENU_ID_01"), _T("CopyShell"), MB_OK);
		break;
	case MENU_ID_2:
		MessageBox(NULL, _T("MENU_ID_02"), _T("CopyShell"), MB_OK);
		break;
	case MENU_ID_3:
		MessageBox(NULL, _T("MENU_ID_03"), _T("CopyShell"), MB_OK);
		break;
	case MENU_ID_4:
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