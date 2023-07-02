#include <Windows.h>

LRESULT CALLBACK 
MainWindowCallback(HWND Window,   // handle to the window
                   UINT Message,  // message identifier
                   WPARAM wParam, // additional message information
                   LPARAM lParam) // additional message information
{
    LRESULT Result = 0; // return value for any message that the window procedure does not process
    switch (Message)
    {
        case WM_SIZE: // sent to a window after its size has changed
        {
            OutputDebugStringA("WM_SIZE\n");
        }break;

        case WM_DESTROY: // sent when a window is being destroyed
        {
            OutputDebugStringA("WM_DESTROY\n");
        }break;
        
        case WM_CLOSE: // sent as a signal that a window or an application should terminate
        {
            DestroyWindow(Window); // Close the window
            OutputDebugStringA("WM_CLOSE\n");
        }break;

        case WM_ACTIVATEAPP: // sent when a window belonging to a different application than the active window is about to be activated    
        {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        }break;

        case WM_PAINT:   
        {
            PAINTSTRUCT Paint; // structure that contains information for an application. This information can be used to paint the client area of a window owned by that application
            HDC DeviceContext = BeginPaint(Window, &Paint); // prepares the specified window for painting and fills a PAINTSTRUCT structure with information about the painting

    // function that performs a bit-block transfer of the color data corresponding to a rectangle of pixels from the specified source device context into a destination device context

            PatBlt(DeviceContext,
                   Paint.rcPaint.left,
                   Paint.rcPaint.top,
                   Paint.rcPaint.right - Paint.rcPaint.left, // width
                   Paint.rcPaint.bottom - Paint.rcPaint.top, // height
                   BLACKNESS); 

            EndPaint(Window, &Paint); // marks the end of painting in the specified window. This function is required for each call to the BeginPaint function, but only after painting is complete
            
        }break;

        default:
        {
            OutputDebugStringA("default\n");
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
    WindowClass.lpfnWndProc = MainWindowCallback ;   // pointer to the window procedure 
    WindowClass.hInstance =  hInstance;
    WindowClass.lpszClassName = "TestWindowClass";


// registers a window class for subsequent use in calls to the CreateWindow or CreateWindowEx function
    if (RegisterClass(&WindowClass))
    { 
            HWND WindowHandle =
                CreateWindowEx(
                    0,                                // extended window style
                    WindowClass.lpszClassName,        // pointer to a null-terminated string or is an atom
                    "Test Window",                    // pointer to a null-terminated string that specifies the window name
                    WS_OVERLAPPEDWINDOW | WS_VISIBLE, // window style
                    CW_USEDEFAULT,                    // initial horizontal position of the window
                    CW_USEDEFAULT,                    // initial vertical position of the window
                    CW_USEDEFAULT,                    // width, in device units, of the window
                    CW_USEDEFAULT,                    // height, in device units, of the window
                    0,                                // handle to the parent or owner window
                    0,                                // handle to a menu, or specifies a child-window identifier depending on the window style
                    hInstance,                        // handle to the instance of the module to be associated with the window
                    0);                               // pointer to a value to be passed to the window through the CREATESTRUCT structure (lpCreateParams member) pointed to by the lParam param of the WM_CREATE message
            if (WindowHandle)  // if the window was created successfully
            {
                MSG Message; // structure that contains message information from a thread's message queue
                for(;;)
                {
                    BOOL MessageResult = GetMessage(&Message,0, 0, 0);
                    if (MessageResult > 0)
                    {
                        TranslateMessage(&Message); // translates virtual-key messages into character messages
                        DispatchMessage(&Message); // dispatches a message to a window procedure
                    }
                    else
                    {
                        break;
                    }

                }       
                
            }
            else
            {

            }

            return (0);
    }else
    {

    }
    return (0);
}