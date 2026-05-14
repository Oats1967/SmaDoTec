// Computergenerierte IDispatch-Wrapperklassen, die mit dem Assistenten zum Hinzufügen von Klassen aus der Typenbibliothek erstellt wurden

// CTouchKeyboardAutomation Wrapperklasse

class CTouchKeyboardAutomation : public COleDispatchDriver
{
public:
    CTouchKeyboardAutomation(){} // Ruft den COleDispatchDriver-Standardkonstruktor auf
    CTouchKeyboardAutomation(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    CTouchKeyboardAutomation(const CTouchKeyboardAutomation& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

    // Attribute
public:

    // Vorgänge
public:


    // ITouchKeyboardAutomation Methoden
public:
    void SmallMode()
    {
        InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    void NormalMode()
    {
        InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    void Show()
    {
        InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    void Hide()
    {
        InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    BOOL get_SearchEnabled()
    {
        BOOL result;
        InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    void put_SearchEnabled(BOOL newValue)
    {
        static BYTE parms[] = VTS_BOOL ;
        InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    void Simulate(LPCTSTR simulatetext)
    {
        static BYTE parms[] = VTS_BSTR ;
        InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, simulatetext);
    }
    long get_CurrentFocusHandle()
    {
        long result;
        InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
        return result;
    }
    void put_CurrentFocusHandle(long newValue)
    {
        static BYTE parms[] = VTS_I4 ;
        InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    long get_CurrentWindowHandle()
    {
        long result;
        InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
        return result;
    }
    void put_CurrentWindowHandle(long newValue)
    {
        static BYTE parms[] = VTS_I4 ;
        InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    BOOL get_StupidEdit()
    {
        BOOL result;
        InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    void put_StupidEdit(BOOL newValue)
    {
        static BYTE parms[] = VTS_BOOL ;
        InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    BOOL get_RepeatedSimulation()
    {
        BOOL result;
        InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    void put_RepeatedSimulation(BOOL newValue)
    {
        static BYTE parms[] = VTS_BOOL ;
        InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    long get_SimulateKind()
    {
        long result;
        InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
        return result;
    }
    void put_SimulateKind(long newValue)
    {
        static BYTE parms[] = VTS_I4 ;
        InvokeHelper(0xd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    long get_LetterStyle()
    {
        long result;
        InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
        return result;
    }
    void put_LetterStyle(long newValue)
    {
        static BYTE parms[] = VTS_I4 ;
        InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    void LoadOptions()
    {
        InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    long get_Left()
    {
        long result;
        InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
        return result;
    }
    void put_Left(long newValue)
    {
        static BYTE parms[] = VTS_I4 ;
        InvokeHelper(0x10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    long get_Top()
    {
        long result;
        InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
        return result;
    }
    void put_Top(long newValue)
    {
        static BYTE parms[] = VTS_I4 ;
        InvokeHelper(0x11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    long get_Width()
    {
        long result;
        InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
        return result;
    }
    void put_Width(long newValue)
    {
        static BYTE parms[] = VTS_I4 ;
        InvokeHelper(0x12, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    long get_Height()
    {
        long result;
        InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
        return result;
    }
    void put_Height(long newValue)
    {
        static BYTE parms[] = VTS_I4 ;
        InvokeHelper(0x13, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    long get_ClientWidth()
    {
        long result;
        InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
        return result;
    }
    void put_ClientWidth(long newValue)
    {
        static BYTE parms[] = VTS_I4 ;
        InvokeHelper(0x14, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    long get_ClientHeight()
    {
        long result;
        InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
        return result;
    }
    void put_ClientHeight(long newValue)
    {
        static BYTE parms[] = VTS_I4 ;
        InvokeHelper(0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    long get_SmallLeft()
    {
        long result;
        InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
        return result;
    }
    void put_SmallLeft(long newValue)
    {
        static BYTE parms[] = VTS_I4 ;
        InvokeHelper(0x16, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    long get_SmallTop()
    {
        long result;
        InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
        return result;
    }
    void put_SmallTop(long newValue)
    {
        static BYTE parms[] = VTS_I4 ;
        InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    long get_SmallWidth()
    {
        long result;
        InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
        return result;
    }
    void put_SmallWidth(long newValue)
    {
        static BYTE parms[] = VTS_I4 ;
        InvokeHelper(0x18, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    long get_SmallHeight()
    {
        long result;
        InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
        return result;
    }
    void put_SmallHeight(long newValue)
    {
        static BYTE parms[] = VTS_I4 ;
        InvokeHelper(0x19, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    long get_SmallClientWidth()
    {
        long result;
        InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
        return result;
    }
    void put_SmallClientWidth(long newValue)
    {
        static BYTE parms[] = VTS_I4 ;
        InvokeHelper(0x1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    long get_SmallClientHeight()
    {
        long result;
        InvokeHelper(0x1b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
        return result;
    }
    void put_SmallClientHeight(long newValue)
    {
        static BYTE parms[] = VTS_I4 ;
        InvokeHelper(0x1b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    long get_SmallestLeft()
    {
        long result;
        InvokeHelper(0x1c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
        return result;
    }
    void put_SmallestLeft(long newValue)
    {
        static BYTE parms[] = VTS_I4 ;
        InvokeHelper(0x1c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    long get_SmallestTop()
    {
        long result;
        InvokeHelper(0x1d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
        return result;
    }
    void put_SmallestTop(long newValue)
    {
        static BYTE parms[] = VTS_I4 ;
        InvokeHelper(0x1d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    long get_SmallestWidth()
    {
        long result;
        InvokeHelper(0x1e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
        return result;
    }
    void put_SmallestWidth(long newValue)
    {
        static BYTE parms[] = VTS_I4 ;
        InvokeHelper(0x1e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    long get_SmallestHeight()
    {
        long result;
        InvokeHelper(0x1f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
        return result;
    }
    void put_SmallestHeight(long newValue)
    {
        static BYTE parms[] = VTS_I4 ;
        InvokeHelper(0x1f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    long get_SmallestClientWidth()
    {
        long result;
        InvokeHelper(0x20, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
        return result;
    }
    void put_SmallestClientWidth(long newValue)
    {
        static BYTE parms[] = VTS_I4 ;
        InvokeHelper(0x20, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    long get_SmallestClientHeight()
    {
        long result;
        InvokeHelper(0x21, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
        return result;
    }
    void put_SmallestClientHeight(long newValue)
    {
        static BYTE parms[] = VTS_I4 ;
        InvokeHelper(0x21, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    void MoveUp()
    {
        InvokeHelper(0x22, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    void MoveDown()
    {
        InvokeHelper(0x23, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    void SmallMoveUp()
    {
        InvokeHelper(0x24, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    void SmallMoveDown()
    {
        InvokeHelper(0x25, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    void SmallMoveLeft()
    {
        InvokeHelper(0x26, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    void SmallMoveRight()
    {
        InvokeHelper(0x27, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    void Terminate()
    {
        InvokeHelper(0x28, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    void About()
    {
        InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    BOOL LoadKeyboardLayout(LPCTSTR filename)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR ;
        InvokeHelper(0x8, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, filename);
        return result;
    }
    BOOL SaveKeyboardLayout(LPCTSTR filename)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR ;
        InvokeHelper(0x29, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, filename);
        return result;
    }
    BOOL get_AutoActivate()
    {
        BOOL result;
        InvokeHelper(0x2a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    void put_AutoActivate(BOOL newValue)
    {
        static BYTE parms[] = VTS_BOOL ;
        InvokeHelper(0x2a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    BOOL get_SystemMenu()
    {
        BOOL result;
        InvokeHelper(0x2b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    void put_SystemMenu(BOOL newValue)
    {
        static BYTE parms[] = VTS_BOOL ;
        InvokeHelper(0x2b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    BOOL setKeyboardLayoutByUniqueID(LPCTSTR uniqueID)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR ;
        InvokeHelper(0xc9, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, uniqueID);
        return result;
    }
    CString getLoadedKeyboardLayoutName()
    {
        CString result;
        InvokeHelper(0xca, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
        return result;
    }
    CString getLoadedKeyboardLayoutUniqueID()
    {
        CString result;
        InvokeHelper(0xcb, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
        return result;
    }
    BOOL isPlugInInstalled(LPCTSTR pluginpathandname)
    {
        BOOL result;
        static BYTE parms[] = VTS_BSTR ;
        InvokeHelper(0xcc, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, pluginpathandname);
        return result;
    }
    BOOL get_InSmallMode()
    {
        BOOL result;
        InvokeHelper(0xcd, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    BOOL get_InSmallestMode()
    {
        BOOL result;
        InvokeHelper(0xce, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    CString get_SmallestModeCaption()
    {
        CString result;
        InvokeHelper(0xcf, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
        return result;
    }
    void put_SmallestModeCaption(LPCTSTR newValue)
    {
        static BYTE parms[] = VTS_BSTR ;
        InvokeHelper(0xcf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    long get_ScreenAlign()
    {
        long result;
        InvokeHelper(0xd0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
        return result;
    }
    void put_ScreenAlign(long newValue)
    {
        static BYTE parms[] = VTS_I4 ;
        InvokeHelper(0xd0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    BOOL get_ClickSoundActive()
    {
        BOOL result;
        InvokeHelper(0xd1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
        return result;
    }
    void put_ClickSoundActive(BOOL newValue)
    {
        static BYTE parms[] = VTS_BOOL ;
        InvokeHelper(0xd1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }
    CString get_ClickSoundName()
    {
        CString result;
        InvokeHelper(0xd2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
        return result;
    }
    void put_ClickSoundName(LPCTSTR newValue)
    {
        static BYTE parms[] = VTS_BSTR ;
        InvokeHelper(0xd2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
    }

    // ITouchKeyboardAutomation Eigenschaften
public:

};
