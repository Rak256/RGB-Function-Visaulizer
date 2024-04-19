/*
Language Code for input:

Odd Array Index Values:
    Any integer can be entered

Even Array Index Values:
    Arithmetic operations -
    1) Addition - Assigned number 1
    2) Subtraction - Assigned number 2
    3) Multiplication - Assigned number 3
    4) Division - Assigned number 4

Special Notations (note first index of sequence in the representations is odd):
[1,4,0] - loop variable x
[-1,4,0] - loop variable y

*/


#include <windows.h>
#include <string>
#include <vector>
#include <sstream>

// Global Variables always initialize to 0 or nullptr
bool running{true};
// 
void* buffer_memory;
int buffer_height;
int buffer_width;
BITMAPINFO buffer_bitmap_info;
LRESULT CALLBACK windows_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Input_Box(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HWND g_hTextBox{NULL};
HWND g_hButtonAdd{NULL};
std::vector<int> g_IntArray; 

LRESULT CALLBACK Input_Box(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch(uMsg){
        case WM_CREATE:{
            //Add Text Box
             g_hTextBox = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 50, 50, 200, 25, hwnd, NULL, NULL, NULL);

             //Add Button
             g_hButtonAdd = CreateWindowEx(0, L"BUTTON", L"Add Value", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 270, 50, 100, 25, hwnd, (HMENU)1, NULL, NULL);
        }
        break;

        case WM_COMMAND: {
            if (LOWORD(wParam) == 1){
                //use Character array to specify size of input in terms of digits and store the input in the array
                wchar_t text_buffer[10];
                GetWindowText(g_hTextBox, text_buffer, 256);
                
                //Convert the text into a single string
                std::wstring text(text_buffer);

                    if (g_IntArray.size()% 2 == 0){

                        switch (text[0]){
                            case L'+':{
                                g_IntArray.push_back(1);
                                MessageBox(hwnd, L"Operator Added", L"Parameter accepted", MB_OK | MB_ICONINFORMATION);
                            }
                            break;

                            case L'-':{
                                g_IntArray.push_back(2);
                                MessageBox(hwnd, L"Operator Added", L"Parameter accepted", MB_OK | MB_ICONINFORMATION);
                            }
                            break;

                            case L'*':{
                                g_IntArray.push_back(3);
                                MessageBox(hwnd, L"Operator Added", L"Parameter accepted", MB_OK | MB_ICONINFORMATION);
                            }
                            break;

                            case L'/':{
                                g_IntArray.push_back(4);
                                MessageBox(hwnd, L"Operator Added", L"Parameter accepted", MB_OK | MB_ICONINFORMATION);
                            }
                        }

                    }
                    else{
                    
                    if (text[0] == 'x'){
                     g_IntArray.push_back(1);
                     g_IntArray.push_back(4);
                     g_IntArray.push_back(0);
                     MessageBox(hwnd, L"Value Added", L"Parameter accepted", MB_OK | MB_ICONINFORMATION);
                    }
                    
                    else if (text[0] == 'y'){
                        g_IntArray.push_back(2);
                        g_IntArray.push_back(4);
                        g_IntArray.push_back(0);
                        MessageBox(hwnd, L"Value Added", L"Parameter accepted", MB_OK | MB_ICONINFORMATION);
                    }
                    
                    else{
                    
                    std::wstringstream converter(text);
                    int value{0};
                    
                    if (converter >> value){
                        g_IntArray.push_back(value);
                        MessageBox(hwnd, L"Value Added", L"Parameter accepted", MB_OK | MB_ICONINFORMATION);
                    }

                    else{
                        MessageBox(hwnd, L"Please enter a valid integer", L"Error", MB_OK | MB_ICONINFORMATION);
                    }
                    
                    }

                    }
                    
            }
        }
        break;

        case WM_DESTROY:{
            PostQuitMessage(0);
            break;
        }

        default:{
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    return 0;
}

LRESULT CALLBACK windows_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
LRESULT result{0};
    switch(uMsg){
        case WM_CLOSE:
        case WM_DESTROY: {
            running = false;
            
            //Deallocate memory if it is in use
            if (buffer_memory){
                VirtualFree(buffer_memory, 0, MEM_RELEASE);
            } 
        } break;

        case WM_SIZE:{
            RECT rect;
            GetClientRect(hwnd, &rect);
             buffer_width = rect.right - rect.left;
             buffer_height = rect.bottom - rect.top; 
         int buffer_size = buffer_width*buffer_height*sizeof(unsigned int);

            buffer_bitmap_info.bmiHeader.biSize = sizeof(buffer_bitmap_info.bmiHeader);
            buffer_bitmap_info.bmiHeader.biHeight = buffer_height;
            buffer_bitmap_info.bmiHeader.biWidth = buffer_width;
            buffer_bitmap_info.bmiHeader.biPlanes = 1; //Set to one as modern devices and standards do not need multiple color planes
            buffer_bitmap_info.bmiHeader.biBitCount = 32;

            //Memory efficiency vs. Load efficiency: My game is small so should not have complex load times. Therefore,
            //color compression is not required - choose RGB

            buffer_bitmap_info.bmiHeader.biCompression = BI_RGB;

            if (buffer_memory){
                VirtualFree(buffer_memory, 0, MEM_RELEASE);
            }

            buffer_memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        } break;

        default: 
        result = DefWindowProc(hwnd, uMsg, wParam, lParam);

    }

    return result ;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd){
    //Input Box Window class
    WNDCLASSEX wc{ sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, Input_Box, 0, 0, hInstance, NULL, NULL, NULL, NULL, L"Main Integer Window", NULL};
    
    //Register_wc
    RegisterClassEx(&wc);

    //Create Text_Box main window
    HWND h_text = CreateWindowEx(0, wc.lpszClassName, L"Integer Array Input", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 200, NULL, NULL, hInstance, NULL);

    // Show window
    ShowWindow(h_text, nShowCmd);
    UpdateWindow(h_text);

    //message loop for input box
    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (msg.message == WM_QUIT){
            break;
        }
    }



    //Window Class
    WNDCLASS window_class = {};
    window_class.style = CS_VREDRAW | CS_HREDRAW;
    window_class.lpszClassName = TEXT("Game Window Class");
    window_class.lpfnWndProc = windows_callback;

    //Register Class
    RegisterClass(&window_class);

    //Create Window
    HWND window = CreateWindow(window_class.lpszClassName, TEXT("Platformer"), WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
    1280, 720, 0, 0, hInstance,0);

    HDC hdc = GetDC(window);

    while(running){
        //Input 
        MSG message;
        
        while(PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)){
            if (!IsDialogMessage(h_text, &message)){
            TranslateMessage(&message);
            DispatchMessage(&message);
            }
        }
        //Simulate
        unsigned int* pixel = (unsigned int*) buffer_memory;
        
        //loop through rows and columns
        for (int y{0}; y < buffer_height; ++y){
            for (int x{0}; x < buffer_width; ++x){
                int result{0};
                
                
                for(int k{1}; k <= g_IntArray.size();){
                    int term{1};
                    bool subtract{false};
                    //divide by zero check
                    if (g_IntArray[0] == 4 && g_IntArray[1] == 0 ){
                        break;
                    }

                    // -1: checks the operator
                    if (g_IntArray[k-1] == 1){
                        if(g_IntArray[k] == 1 && g_IntArray[k+1] == 4 && g_IntArray [k+2] == 0){
                            //get to next value
                            k += 4;
                            //only add when confirmed that term is complete
                            if (g_IntArray[k-1] == 1 || g_IntArray[k-1] == 2 ){
                            result += x;
                        }

                        else if(g_IntArray[k-1] == 3){
                            term *= x;
                        }

                        else{
                            term /= x;
                        }
                        }
                        else if (g_IntArray[k] == 2 && g_IntArray[k+1] == 4 && g_IntArray [k+2] == 0){
                            //get to next value
                            k += 4;
                            if (g_IntArray[k-1] == 1 ||g_IntArray[k-1] == 2 ){
                            result += y;
                            }
                            else if(g_IntArray[k-1] == 3){
                            term *= y;
                        }

                        else{
                            term /= y;
                        }
                        }

                        else{
                            //get to next value
                            k += 2;

                           if (g_IntArray[k-1] == 1 ||g_IntArray[k-1] == 2 ){
                            result += g_IntArray[k];
                            }

                            else if(g_IntArray[k-1] == 3){
                            term *= g_IntArray[k];
                        }

                        else{
                            term /= g_IntArray[k];
                        }
                        }
                }

                 else if (g_IntArray[k-1] == 2){
                            if(g_IntArray[k] == 1 && g_IntArray[k+1] == 4 && g_IntArray [k+2] == 0){
                             k += 4;
                            if (g_IntArray[k-1] == 3 || g_IntArray[k-1] == 4){
                               //check if the term is being negated. if so, set subtract to true 
                               subtract = true;

                               if(g_IntArray[k-1] == 3){
                                term *= x;
                               }

                               else{
                                term /= x;
                               }
                            }
            
                            else{
                             result -= x;
                            }
                            }
                            else if (g_IntArray[k] == 2 && g_IntArray[k+1] == 4 && g_IntArray [k+2] == 0){
                                k += 4;
                            if (g_IntArray[k-1] == 3 || g_IntArray[k-1] == 4){
                               //check if the term is being negated. if so, set subtract to true 
                               subtract = true;

                               if(g_IntArray[k-1] == 3){
                                term *= x;
                               }

                               else{
                                term /= x;
                               }
                            }

                            else{
                                result -= y;
                            }  
                            }

                            else{
                                k += 2;
                            if (g_IntArray[k-1] == 3 || g_IntArray[k-1] == 4){
                               //check if the term is being negated. if so, set subtract to true 
                               subtract = true;

                               if(g_IntArray[k-1] == 3){
                                term *= x;
                               }

                               else{
                                term /= x;
                               }
                            }

                            else{
                                result -= g_IntArray[k];
                            }
                            }
                }

                 if (g_IntArray[k-1] == 3){
                    // loop until k-1 = 1 or k-1 = 2 or k is outside vector size 
                    //(exit multiplication/division loop when a term in the expression is complete)
                    while ((g_IntArray[k-1] != 2) && (g_IntArray[k-1] != 1)){
                        //multiplication or division? check!
                        if (g_IntArray[k-1] == 3){
                            if(g_IntArray[k] == 1 && g_IntArray[k+1] == 4 && g_IntArray [k+2] == 0){
                             term *= x;
                             k += 4;
                             if (k > g_IntArray.size()){
                                break;
                             }
                            }
                            else if (g_IntArray[k] == 2 && g_IntArray[k+1] == 4 && g_IntArray [k+2] == 0){
                                term *= y;
                                k += 4;
                                if (k > g_IntArray.size()){
                                break;
                             }
                            }

                            else{
                                term *= g_IntArray[k];
                                k += 2;
                                 if (k > g_IntArray.size()){
                                break;
                             }
                            }
                    }
                        else{
                            if(g_IntArray[k] == 1 && g_IntArray[k+1] == 4 && g_IntArray [k+2] == 0){
                             term /= x;
                             k += 4;
                              if (k > g_IntArray.size()){
                                break;
                             }
                            }
                            else if (g_IntArray[k] == 2 && g_IntArray[k+1] == 4 && g_IntArray [k+2] == 0){
                                term /= y;
                                k += 4;
                                 if (k > g_IntArray.size()){
                                break;
                             }
                            }

                            else{
                                term /= g_IntArray[k];
                                k += 2;
                                 if (k > g_IntArray.size()){
                                break;
                             }
                            }
                        }
                }
                    if (subtract){
                        result -= term;
                    }

                    else{
                        result += term;
                    }

                }

                else if (g_IntArray[k-1] == 4){
                    if (g_IntArray[k] == 0){
                        break;
                    }

                    else{
                        while ((g_IntArray[k-1] != 2) && (g_IntArray[k-1] != 1)&& k <= g_IntArray.size()){
                        //multiplication or division? check!
                        if (g_IntArray[k-1] == 3){
                            if(g_IntArray[k] == 1 && g_IntArray[k+1] == 4 && g_IntArray [k+2] == 0){
                             term *= x;
                             k += 4;
                              if (k > g_IntArray.size()){
                                break;
                             }
                            }
                            else if (g_IntArray[k] == 2 && g_IntArray[k+1] == 4 && g_IntArray [k+2] == 0){
                                term *= y;
                                k += 4;
                                if (k > g_IntArray.size()){
                                break;
                             }
                            }

                            else{
                                term *= g_IntArray[k];
                                k += 2;
                                if (k > g_IntArray.size()){
                                break;
                             }
                            }
                    }
                        else{
                            if(g_IntArray[k] == 1 && g_IntArray[k+1] == 4 && g_IntArray [k+2] == 0){
                             term /= x;
                             k += 4;
                              if (k > g_IntArray.size()){
                                break;
                             }
                            }
                            else if (g_IntArray[k] == 2 && g_IntArray[k+1] == 4 && g_IntArray [k+2] == 0){
                                term /= y;
                                k += 4;
                                if (k > g_IntArray.size()){
                                break;
                             }
                            }

                            else{
                                term /= g_IntArray[k];
                                k += 2;
                                if (k > g_IntArray.size()){
                                break;
                             }
                            }
                        }
                }
                    if (subtract){
                        result -= term;
                    }

                    else{
                        result += term;
                    }
                    }

                }
            
        }
                *pixel++ = result;
            }
        }
        // Getting rid of Dangling Pointer - DO NOT REMOVE THIS CODE AS IT WILL LEAD TO PROGRAM TERMINATION IF 'pixel' IS ACCESSED AGAIN
        pixel = nullptr;
        
        //Render
        StretchDIBits(hdc, 0, 0, buffer_width, buffer_height, 0, 0, buffer_width, buffer_height, buffer_memory, &buffer_bitmap_info, DIB_RGB_COLORS, SRCCOPY);
    }

    return 0;
}

