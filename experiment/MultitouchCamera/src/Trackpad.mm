#import "Trackpad.h"
#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>

trackpad_rotate_callback_func trackpad_rotate_callback_function(0);
trackpad_swipe_callback_func trackpad_swipe_callback_function(0);
trackpad_magnify_callback_func trackpad_magnify_callback_function(0);
trackpad_begin_gesture_callback_func trackpad_begin_gesture_callback_function(0);
trackpad_end_gesture_callback_func trackpad_end_gesture_callback_function(0);
trackpad_mouse_move_callback_func trackpad_mouse_move_callback_function(0);
trackpad_mouse_scroll_callback_func trackpad_mouse_scroll_callback_function(0);
trackpad_mouse_down_callback_func trackpad_left_mouse_down_callback_function(0);
trackpad_mouse_down_callback_func trackpad_right_mouse_down_callback_function(0);
trackpad_mouse_up_callback_func trackpad_left_mouse_up_callback_function(0);
trackpad_mouse_up_callback_func trackpad_right_mouse_up_callback_function(0);
trackpad_mouse_dragged_callback_func trackpad_left_mouse_dragged_callback_function(0);
trackpad_mouse_dragged_callback_func trackpad_right_mouse_dragged_callback_function(0);

void trackpad_set_rotate_callback(trackpad_rotate_callback_func func) {
	trackpad_rotate_callback_function = func;
}

void trackpad_set_swipe_callback(trackpad_swipe_callback_func func) {
	trackpad_swipe_callback_function = func;
}

void trackpad_set_magnify_callback(trackpad_magnify_callback_func func) {
	trackpad_magnify_callback_function = func;
}

void trackpad_set_begin_gesture_callback(trackpad_begin_gesture_callback_func func) {
	trackpad_begin_gesture_callback_function = func;
}

void trackpad_set_end_gesture_callback(trackpad_end_gesture_callback_func func) {
	trackpad_end_gesture_callback_function = func;
}

void trackpad_set_mouse_move_callback(trackpad_mouse_move_callback_func func) {
	trackpad_mouse_move_callback_function = func;
}

void trackpad_set_mouse_scroll_callback(trackpad_mouse_scroll_callback_func func) {
	trackpad_mouse_scroll_callback_function = func;
}

void trackpad_set_left_mouse_down_callback(trackpad_mouse_down_callback_func func) {
	trackpad_left_mouse_down_callback_function = func;
}

void trackpad_set_right_mouse_down_callback(trackpad_mouse_down_callback_func func) {
	trackpad_right_mouse_down_callback_function = func;
}

void trackpad_set_left_mouse_up_callback(trackpad_mouse_up_callback_func func) {
	trackpad_left_mouse_up_callback_function = func;
}

void trackpad_set_right_mouse_up_callback(trackpad_mouse_up_callback_func func) {
	trackpad_right_mouse_up_callback_function = func;
}

void trackpad_set_left_mouse_dragged_callback(trackpad_mouse_dragged_callback_func func) {
	trackpad_left_mouse_dragged_callback_function = func;
	
}

void trackpad_set_right_mouse_dragged_callback(trackpad_mouse_dragged_callback_func func) {
	trackpad_right_mouse_dragged_callback_function = func;
}

// -----------------------------------------------------------------------------
void trackpad_handle_event(NSEvent* event) {
	switch([event type]) {
		case NSEventTypeRotate: {
			if(trackpad_rotate_callback_function != 0) {
				trackpad_rotate_callback_function([event rotation]);
			}
			break;
		}
		case NSEventTypeSwipe: {
			if(trackpad_swipe_callback_function != 0) {
				trackpad_swipe_callback_function([event deltaX], [event deltaY]);
			}
			break;
		}
		case NSEventTypeMagnify: {
			if(trackpad_magnify_callback_function != 0) {
				trackpad_magnify_callback_function([event magnification]);
			}
			break;
		}
		case NSEventTypeBeginGesture: {
			if(trackpad_begin_gesture_callback_function != 0) {
				trackpad_begin_gesture_callback_function();
			}
			break;
		}
		case NSEventTypeEndGesture: {
			if(trackpad_end_gesture_callback_function != 0) {
				trackpad_end_gesture_callback_function();
			}
			break;
		}
		case NSLeftMouseDown: {
			if(trackpad_left_mouse_down_callback_function != 0
			    && [event subtype] == NSTouchEventSubtype) 
			{
				NSPoint pt = [event locationInWindow];
				trackpad_left_mouse_down_callback_function(pt.x, pt.y);
			}
			break;
		}
		case NSRightMouseDown: {
			if(trackpad_right_mouse_down_callback_function != 0
			   && [event subtype] == NSTouchEventSubtype) 
			{
				NSPoint pt = [event locationInWindow];
				trackpad_right_mouse_down_callback_function(pt.x, pt.y);
			}
			break;
		}	
		case NSLeftMouseUp: {
			if(trackpad_left_mouse_up_callback_function != 0
			    && [event subtype] == NSTouchEventSubtype)
			{
				NSPoint pt = [event locationInWindow];
				trackpad_left_mouse_up_callback_function(pt.x, pt.y);
			}
			break;
		}
		case NSRightMouseUp: {
			if(trackpad_right_mouse_up_callback_function != 0
			    && [event subtype] == NSTouchEventSubtype)
			{
				NSPoint pt = [event locationInWindow];
				trackpad_right_mouse_up_callback_function(pt.x, pt.y);
			}
			break;
		}	
		case NSScrollWheel: {
			if(trackpad_mouse_scroll_callback_function != 0
				&& [event subtype] == NSTabletPointEventSubtype    
			)
			{
				trackpad_mouse_scroll_callback_function([event deltaX], [event deltaY]);
			}
			break;
		}
		case NSLeftMouseDragged: {
			NSPoint pt = [event locationInWindow];
			if(trackpad_left_mouse_dragged_callback_function != 0
			    && [event subtype] == NSTouchEventSubtype) 
			{
				trackpad_left_mouse_dragged_callback_function(pt.x, pt.y, [event deltaX], [event deltaY]);
			}
			break;
		}
		case NSRightMouseDragged: {
			NSPoint pt = [event locationInWindow];
			if(trackpad_right_mouse_dragged_callback_function != 0
			    && [event subtype] == NSTouchEventSubtype) 
			{
				trackpad_right_mouse_dragged_callback_function(pt.x, pt.y, [event deltaX], [event deltaY]);
			}
			break;
		}
		case NSMouseMoved: {
			if(trackpad_mouse_move_callback_function != 0
			    && [event subtype] == NSTouchEventSubtype) 
			{
				NSPoint pt = [event locationInWindow];
				trackpad_mouse_move_callback_function(pt.x, pt.y);
			}
			break;
		}
		
    		default:break;
    };
}

void trackpad_init() {
	NSUInteger mask = 	NSEventMaskBeginGesture 
						| NSEventMaskEndGesture 
						| NSEventMaskMagnify 
						| NSEventMaskSwipe 
						| NSEventMaskRotate 
						| NSEventMaskGesture
						| NSScrollWheelMask
						| NSMouseMovedMask
						| NSLeftMouseDownMask
						| NSRightMouseDownMask
						| NSLeftMouseUpMask
						| NSRightMouseUpMask
						| NSLeftMouseDraggedMask
						| NSRightMouseDraggedMask
						;
						
	[NSEvent    addGlobalMonitorForEventsMatchingMask:mask
                handler:^(NSEvent* event)
    {
       trackpad_handle_event(event); 
    }];	
    
    
    [NSEvent    addLocalMonitorForEventsMatchingMask:mask
                handler:^(NSEvent* event)
    {
    		trackpad_handle_event(event);
    		return event;
    }];
		
}

/* Code snippets:
// trackpad: http://developer.apple.com/library/mac/#documentation/Cocoa/Conceptual/EventOverview/HandlingTouchEvents/HandlingTouchEvents.html#//apple_ref/doc/uid/10000060i-CH13-SW10
// masks: http://developer.apple.com/library/mac/#documentation/Cocoa/Reference/ApplicationKit/Classes/NSEvent_Class/Reference/Reference.html#//apple_ref/c/econst/NSTouchEventSubtype
// @see http://developer.apple.com/library/mac/#documentation/Cocoa/Reference/ApplicationKit/Classes/NSEvent_Class/Reference/Reference.html#//apple_ref/c/tdef/NSEventType

//printf("Type: %d, %d -- %d", [event subtype], NSTouchEventSubtype, NSTabletPointEventSubtype   );
//printf("Type: %d, %d", [event subtype], NSTouchEventSubtype);

case NSMouseExited: {
	break;
}
case NSEventTypeGesture: {
	break;
}

	enum TouchPhase {
		 TOUCH_PHASE_BEGAN			=	1 << 0
		,TOUCH_PHASE_MOVED			=	1 << 1
		,TOUCH_PHASE_STATIONARY		=	1 << 2
		,TOUCH_PHASE_ENDED			=	1 << 3
		,TOUCH_PHASE_CANCELLED		= 	1 << 4
		
		,TOUCH_PHASE_TOUCHING			= TOUCH_PHASE_BEGAN | TOUCH_PHASE_MOVED | TOUCH_PHASE_STATIONARY
		,TOUCH_PHASE_ANY				= 0xFF
	};
*/