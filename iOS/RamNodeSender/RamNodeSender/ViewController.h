//
//  ViewController.h
//  RamNodeSender
//
//  Created by 神田竜 on 2014/05/09.
//  Copyright (c) 2014年 神田竜. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "VVOSC.h"
#import <CoreMotion/CoreMotion.h>
#include "Quaternion.h"

@interface ViewController : UIViewController
@property (weak, nonatomic) IBOutlet UITextField *hostTextField;
@property (weak, nonatomic) IBOutlet UITextField *portTextField;
@property CMMotionManager *motionManager;
@property NSTimeInterval startTime;
@property OSCManager *manager;
@property OSCOutPort *outPort;
@property (weak, nonatomic) IBOutlet UILabel *pitch;
@property (weak, nonatomic) IBOutlet UILabel *roll;
@property (weak, nonatomic) IBOutlet UILabel *yaw;

-(void)saveUseDefaults;

@end
