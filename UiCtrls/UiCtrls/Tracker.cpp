#include "stdafx.h"
#include "Tracker.h"

bool TrackDragAndDrop(CTracker& tracker, HWND hWnd)
{
    bool bResult=true;
    tracker.BeginDrag();
    ::SetCapture(hWnd);
    MSG msg;
    while((::GetCapture()==hWnd)&&
            (GetMessage(&msg, NULL, 0, 0)))
    {
		if(!tracker.ProcessWindowMessage(&msg))
		{
		  switch(msg.message)
		  {
			   case WM_MOUSEMOVE:
					tracker.OnMove(GET_X_LPARAM( msg.lParam), GET_Y_LPARAM(msg.lParam));
					break;
			   case WM_RBUTTONUP:
					::ReleaseCapture();
					bResult=tracker.OnDropRightButton(GET_X_LPARAM( msg.lParam), GET_Y_LPARAM(msg.lParam));
					break;
			   case WM_LBUTTONUP:
					::ReleaseCapture();
					bResult=tracker.OnDropLeftButton(GET_X_LPARAM( msg.lParam), GET_Y_LPARAM(msg.lParam));
					break;
			   case WM_KEYDOWN:
					if(msg.wParam!=VK_ESCAPE)
						break;
			   case WM_RBUTTONDOWN:
			   case WM_LBUTTONDOWN:
					::ReleaseCapture();
					tracker.OnCancelDrag(GET_X_LPARAM( msg.lParam), GET_Y_LPARAM(msg.lParam));
					bResult=false;
					break;
			   case WM_SYSKEYDOWN:
					::ReleaseCapture();
					tracker.OnCancelDrag(GET_X_LPARAM( msg.lParam), GET_Y_LPARAM(msg.lParam));
					bResult=false;
			   default:
					DispatchMessage(&msg);
		  }
		}
    }
    tracker.EndDrag(!bResult);
    ASSERT(::GetCapture()!=hWnd);
    return bResult;
}

void TrackDragAndDrop(CCallBackListener&listener, HWND hWnd, POINT startPoint, bool lockWindowUpdate/* = false*/, HWND clientViewWnd/* = NULL*/)
{
	ASSERT(::IsWindow( hWnd));

	if (!::DragDetect( hWnd, startPoint))
		return;

	// tracker drawing conflicts with dock pane drawing
	// disable drawing in the window during drag and drop operations.
	if ( lockWindowUpdate)
		::LockWindowUpdate( hWnd);

	MSG msg;
	bool dragging = false;
	::SetCapture( hWnd);
	while((::GetCapture()==hWnd) && (::GetMessage( &msg, NULL, 0, 0)))
	{			
		CPoint hitPoint = CPoint( GET_X_LPARAM( msg.lParam), GET_Y_LPARAM( msg.lParam));
		::ClientToScreen( hWnd, &hitPoint);
		switch( msg.message) {
		case WM_MOUSEMOVE:
			{
				if (!dragging)
				{
					dragging = true;
					listener.DragStart( hWnd, clientViewWnd, hitPoint.x, hitPoint.y, static_cast<DWORD>(msg.wParam));
				}

				bool cancelDrag = false;
				listener.DragOver( hWnd, clientViewWnd, hitPoint.x, hitPoint.y, static_cast<DWORD>(msg.wParam), cancelDrag );

				if (cancelDrag)
				{
					dragging = false;
					listener.DragCancel(hWnd, NULL);
					::ReleaseCapture();
				}
			}

			break;

		case WM_LBUTTONUP:

			if (dragging)
			{
				dragging = false;
				listener.DragDrop( hWnd, clientViewWnd, hitPoint.x, hitPoint.y, static_cast<DWORD>(msg.wParam));
			}
			::ReleaseCapture();
			break;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:

			switch (msg.wParam)
			{
			case VK_CONTROL:
			case VK_SHIFT:
				break;
			default:
				if (dragging)
				{
					dragging = false;
					listener.DragCancel(hWnd, NULL);
				}
				::ReleaseCapture();
			}
		default:
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	// tidy up
	if (dragging)
		listener.DragCancel( hWnd, clientViewWnd);

	if (lockWindowUpdate)
		::LockWindowUpdate( NULL);
}