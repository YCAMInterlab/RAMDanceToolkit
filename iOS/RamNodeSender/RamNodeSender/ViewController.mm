//
//  ViewController.m
//  RamNodeSender
//
//  Created by 神田竜 on 2014/05/09.
//  Copyright (c) 2014年 神田竜. All rights reserved.
//

#import "ViewController.h"

#define PI 3.14159265

@interface ViewController ()

@end

@implementation ViewController


- (IBAction)hostDidEndOnExit:(id)sender
{
    NSLog(@"%@", self.hostTextField.text);
    [_manager deleteAllOutputs];
    _outPort = [_manager createNewOutputToAddress:self.hostTextField.text
                                           atPort:[self.portTextField.text intValue]];
    [self saveUseDefaults];

 //   [sender resignFirstResponder];
}

- (IBAction)portDidEndOnExit:(id)sender
{
    NSLog(@"%@", self.portTextField.text);
    [_manager deleteAllOutputs];
    _outPort = [_manager createNewOutputToAddress:self.hostTextField.text
                                           atPort:[self.portTextField.text intValue]];
    [self saveUseDefaults];
 //   [sender resignFirstResponder];
}

- (void)saveUseDefaults{
    NSArray *array = @[self.hostTextField.text, self.portTextField.text];
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:array forKey:@"HostAndPort"];
    BOOL successful = [defaults synchronize];
    if (successful) {
        NSLog(@"%@", @"データの保存に成功しました。");
    }
}


- (void)viewDidLoad
{
    
    [super viewDidLoad];
    
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSArray *array = [defaults arrayForKey:@"HostAndPort"];
    if (array) {
        
        self.hostTextField.text = [array objectAtIndex:0];
        self.portTextField.text = [array objectAtIndex:1];
        
    } else {
        
        self.hostTextField.text = @"169.254.100.100";
        self.portTextField.text = @"10000";
        
        NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
        NSArray *array = @[self.hostTextField.text, self.portTextField.text];
        [defaults setObject:array forKey:@"HostAndPort"];
    }

    
    _startTime = [[[NSDate alloc] init] timeIntervalSince1970];
    
	// Do any additional setup after loading the view, typically from a nib.
    _manager = [[OSCManager alloc] init];
    _manager.delegate = self;
    
    _outPort = [_manager createNewOutputToAddress:self.hostTextField.text atPort:[self.portTextField.text intValue]];
    
    _motionManager = [[CMMotionManager alloc] init];
    
    [_motionManager startDeviceMotionUpdatesToQueue:[NSOperationQueue currentQueue]
                                       withHandler:^(CMDeviceMotion *motion, NSError *error)
     {
         
         float dt = ([[[NSDate alloc] init] timeIntervalSince1970] - _startTime);
         
         Quaternion quat;
         quat.setEuler(motion.attitude.yaw,
                       motion.attitude.pitch,
                       motion.attitude.roll);
         
         //NSLog(@"%f",motion.attitude.yaw);
         
         float sinhalfangle = sqrt( quat.x * quat.x + quat.y * quat.y + quat.z * quat.z );
         
         float angle = 2.0 * atan2( sinhalfangle, quat.w );
         if (sinhalfangle) {
             quat.x = quat.x / sinhalfangle;
             quat.y = quat.y / sinhalfangle;
             quat.z = quat.z / sinhalfangle;
         } else {
             quat.x = 0.0;
             quat.y = 0.0;
             quat.z = 1.0;
         }
         
         angle = angle * 180.0 / 3.14159265;
         
         OSCMessage *message = [OSCMessage createWithAddress:@"/ram/rigid_body"];
         [message addString:@"iPhone"];
         [message addInt:1];
         [message addString:@"Neck"];
         [message addFloat:0.0];
         [message addFloat:0.0];
         [message addFloat:0.0];
         [message addFloat:angle];
         [message addFloat:quat.x];
         [message addFloat:quat.y];
         [message addFloat:quat.z];
         [message addFloat:dt];
         [_outPort sendThisPacket:[OSCPacket createWithContent:message]];
        
         self.pitch.text = [NSString stringWithFormat:@"%f",motion.attitude.pitch * 180 / PI];
         self.roll.text = [NSString stringWithFormat:@"%f",motion.attitude.roll * 180 / PI];
         self.yaw.text = [NSString stringWithFormat:@"%f",motion.attitude.yaw * 180 / PI];
         
     }];

}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
