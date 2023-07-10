#include <Windows.h>
#include <stdint.h>

#define file_local static // static function
#define local_persist static // static variable
#define global_variable static

/* global variables */
global_variable bool running; // false by default
 // structure that defines the dimensions and color information for a Windows device-independent bitmap (DIB), clear to 0 by default
global_variable BITMAPINFO BitmapInfo;
global_variable void *BitmapMemory; // pointer to the bitmap bits
global_variable int BitmapWidth ;
global_variable int BitmapHeight ;
global_variable int BytesPerPixel = 4; // 4 bytes per pixel


file_local void
RenderGradient(int XOffset, int YOffset)
{
    int Width = BitmapWidth;
    int Height = BitmapHeight;
    int Pitch = Width*BytesPerPixel; // number of bytes in one scan line of the bitmap
    uint8_t *Row = (uint8_t *)BitmapMemory; // pointer to the first row of the bitmap
    for(int Y = 0; Y < BitmapHeight; ++Y)
    {
        uint32_t *Pixel = (uint32_t *)Row; // pointer to the first pixel of the row
        for(int X = 0; X < BitmapWidth; ++X)
        {
            /*
                Pixel in memory: 0x xx RR GG BB 
                little endian:   0x BB GG RR xx
             */

            uint8_t Blue = (X + XOffset);
            uint8_t Green = (Y + YOffset);

            *Pixel++ = ((Green << 8) | Blue);
        }
        Row += Pitch; // moves the pointer to the next row
    }
}


/** DIB (Device Independent Bitmap) Section */
file_local void 
Win32ResizeDIBSection(int Width, int Height)
{  
    if (BitmapMemory)
    {
        VirtualFree(BitmapMemory, 0, MEM_RELEASE); // releases, decommits, or releases and decommits a region of pages within the virtual address space of the calling process
    } 
    BitmapWidth = Width;
    BitmapHeight = Height;

    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader); // size, in bytes, of the structure 
    BitmapInfo.bmiHeader.biWidth = BitmapWidth;                 // width, in pixels, of the DIB
    BitmapInfo.bmiHeader.biHeight = -BitmapHeight;              // height, in pixels, of the DIB (negative indicates a top-down DIB)
    BitmapInfo.bmiHeader.biPlanes = 1;                          // number of planes for the target device must be set to 1
    BitmapInfo.bmiHeader.biBitCount = 32;                       // number of bits-per-pixel
    BitmapInfo.bmiHeader.biCompression = BI_RGB;                // uncompressed format

    int BitmapMemorySize = (BitmapWidth*BitmapHeight)*BytesPerPixel; // size of the bitmap buffer in bytes
    BitmapMemory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE); // reserves or commits a region of pages in the virtual address space of the calling process
}

file_local void
Win32UpdateWindow(HDC DeviceContext,RECT *ClientRect, int X, int Y, int Width, int Height)
{
    int WindowWidth = ClientRect->right - ClientRect->left;
    int WindowHeight = ClientRect->bottom - ClientRect->top;

    StretchDIBits(DeviceContext, // handle to the device context
                 /* 
                  X, Y, Width, Height, // destination rectangle
                  X, Y, Width, Height, // source rectangle 
                  */
                  0, 0, BitmapWidth, BitmapHeight, // destination rectangle
                  0, 0, WindowWidth, WindowHeight, // source rectangle
                  BitmapMemory,
                  &BitmapInfo,     // source rectangle
                  DIB_RGB_COLORS,  // color data
                  SRCCOPY);        // raster operation code

}

LRESULT CALLBACK 
Win32MainWindowCallback(HWND Window,   // handle to the window
                   UINT Message,  // message identifier
                   WPARAM wParam, // additional message information
                   LPARAM lParam) // additional message information
{
    LRESULT Result = 0; // return value for any message that the window procedure does not process
    switch (Message)
    {
        case WM_SIZE: // sent to a window after its size has changed
        {
            RECT ClientRect; // structure that defines the coordinates of the upper-left and lower-right corners of a rectangle
            GetClientRect(Window, &ClientRect); // retrieves the coordinates of a window's client area
            int Width = ClientRect.right - ClientRect.left; // width of the rectangle
            int Height = ClientRect.bottom - ClientRect.top; // height of the rectangle
            Win32ResizeDIBSection(Width, Height);
        }break;
        
        case WM_CLOSE: // sent as a signal that a window or an application should terminate
        {
            running = false;
        }break;

        case WM_ACTIVATEAPP: // sent when a window belonging to a different application than the active window is about to be activated    
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        }break;

        case WM_DESTROY: // sent when a window is being destroyed
        {
            running = false;
        }break;

        case WM_PAINT:   
        {
            PAINTSTRUCT Paint; // structure that contains information for an application. This information can be used to paint the client area of a window owned by that application
            HDC DeviceContext = BeginPaint(Window, &Paint); // prepares the specified window for painting and fills a PAINTSTRUCT structure with information about the painting
            
            int X = Paint.rcPaint.left;
            int Y = Paint.rcPaint.top;
            int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
            int Width = Paint.rcPaint.right - Paint.rcPaint.left;

            RECT ClientRect; // structure that defines the coordinates of the upper-left and lower-right corners of a rectangle
            GetClientRect(Window, &ClientRect); // retrieves the coordinates of a window's client area

            Win32UpdateWindow(DeviceContext, &ClientRect,X, Y, Width, Height);

            EndPaint(Window, &Paint); // marks the end of painting in the specified window. This function is required for each call to the BeginPaint function, but only after painting is complete
            
        }break;

        default:
        {
            Result = DefWindowProc(Window, Message, wParam, lParam); // calls the default window procedure to provide default processing for any window messages that an application does not process
        }break;
    }
    return(Result);
}                            


/**
 * The C and C++ standards require any program (for a “hosted” C or C++ implementation)
 * to have a function called main, which serves as the program's startup function.  
 * The WinMain char based function was introduced with Windows, in the early 1980's:
 * Where CALLBACK, HINSTANCE and LPSTR are defined by the <windows.h> header 
 * LPSTR is just a char* (pointer to char)
 */

int CALLBACK
WinMain (HINSTANCE hInstance, // the base address of the executable, primary use is to load resources
         HINSTANCE hPrevInstance, // always = 0
         LPSTR lpCmdLine, // command line arguments
         int nCmdShow) // how the window is to be shown
{
/**
 * The WNDCLASS structure defines the window class
 *   UINT      style;         // style of the window
 *   WNDPROC   lpfnWndProc;   // pointer to the window procedure
 *   int       cbClsExtra;    // number of extra bytes to allocate following the window-class structure
 *   int       cbWndExtra;    // number of extra bytes to allocate following the window instance
 *   HINSTANCE hInstance;     // handle to the instance that contains the window procedure for the class
 *   HICON     hIcon;         // handle to the class icon
 *   HCURSOR   hCursor;       // handle to the class cursor
 *   HBRUSH    hbrBackground; // handle to the class background brush
 *   LPCSTR    lpszMenuName;  // char* that specifies the resource name of the class menu
 *   LPCSTR    lpszClassName; // char* or is an atom 
*/
    WNDCLASS WindowClass = {}; // window class
    WindowClass.style = CS_OWNDC   | //allocates a unique device context for each window in the class
                        CS_HREDRAW | // redraws the entire window if a movement or size adjustment changes the width of the client area
                        CS_VREDRAW; // redraws the entire window if a movement or size adjustment changes the height of the client area
    WindowClass.lpfnWndProc = Win32MainWindowCallback ;   // pointer to the window procedure 
    WindowClass.hInstance =  hInstance;
    WindowClass.lpszClassName = (LPCSTR)"TestWindowClass";


// registers a window class for subsequent use in calls to the CreateWindow or CreateWindowEx function
    if (RegisterClass(&WindowClass))
    { 
            HWND Window =
                CreateWindowEx(
                    0,                                // extended window style
                    WindowClass.lpszClassName,        // pointer to a null-terminated string or is an atom
                    (LPCSTR)"Test Window",                    // pointer to a null-terminated string that specifies the window name
                    WS_OVERLAPPEDWINDOW | WS_VISIBLE, // window style
                    CW_USEDEFAULT,                    // initial horizontal position of the window
                    CW_USEDEFAULT,                    // initial vertical position of the window
                    CW_USEDEFAULT,                    // width, in device units, of the window
                    CW_USEDEFAULT,                    // height, in device units, of the window
                    0,                                // handle to the parent or owner window
                    0,                                // handle to a menu, or specifies a child-window identifier depending on the window style
                    hInstance,                        // handle to the instance of the module to be associated with the window
                    0);                               // pointer to a value to be passed to the window through the CREATESTRUCT structure (lpCreateParams member) pointed to by the lParam param of the WM_CREATE message
            if (Window)  // if the window was created successfully
            {
            /* Message Loop */
                int XOfsset = 0;
                int YOfsset = 0;
                running = true;
                while(running)
                {
                    MSG Message; // structure that contains message information from a thread's message queue
                    while (PeekMessage(&Message,0, 0, 0, PM_REMOVE))
                    {
                        if(Message.message == WM_QUIT)
                        {
                            running = false;
                        }
                        TranslateMessage(&Message); // translates virtual-key messages into character messages
                        DispatchMessage(&Message); // dispatches a message to a window procedure
                    }
                    RenderGradient(XOfsset++, YOfsset++);

                    HDC DeviceContext = GetDC(Window); 
                    RECT ClientRect;
                    GetClientRect(Window, &ClientRect);
                    int WindowWidth = ClientRect.right - ClientRect.left;
                    int WindowHeight = ClientRect.bottom - ClientRect.top;

                    Win32UpdateWindow(DeviceContext, &ClientRect,0,0,WindowWidth, WindowHeight);
                    ReleaseDC(Window, DeviceContext);
                }       
            }
            else
            {

            }
    }
    else
    {

    }
    return (0);
}