#import "GameViewController.h"
#import <OpenGLES/ES2/glext.h>
#import "../../GreenJuiceTeam.Shared/game/Engine.h"

#import <memory>

@interface GameViewController () {}
@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;
- (void)tearDownGL;

@end

@implementation GameViewController
{
    std::unique_ptr<Engine> engine;
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }

    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    //[[UIDevice currentDevice] setValue:@(UIInterfaceOrientationLandscapeLeft) forKey:@"orientation"];

    [self setupGL];
}

- (void)dealloc
{
    [self tearDownGL];

    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;

        [self tearDownGL];

        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }

    // Dispose of any resources that can be recreated.
}

- (BOOL)prefersStatusBarHidden {
    return YES;
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];

	if (engine == nullptr)
	{
		engine.reset(new Engine());
		engine->init(self.view.bounds.size.width, self.view.bounds.size.height);
	}
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];

	engine.reset();
}

#pragma mark - GLKView and GLKViewController delegate methods

extern float _rotation;

- (void)update
{
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesBegan:touches withEvent:event];
    touch = true;
}


#define JOYSTICK_RADIUS 64.0f
#define THUMB_RADIUS 48.0f

bool isPressedPos;
extern bool touch;
int touchDirection;

bool isPointInCircle(Vector2D<float> point, Vector2D<float> center, float radius) {
    float dx = (point.x - center.x);
    float dy = (point.y - center.y);
    return (radius*radius >= (dx * dx) + (dy * dy));
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesMoved:touches withEvent:event];
    UITouch* touchEvent = [touches anyObject];
    CGPoint locationInView = [touchEvent locationInView:self.view];
    int x = locationInView.x;
    int y = locationInView.y;
    x*=480.0f/self.view.bounds.size.height;
    y*=800.0f/self.view.bounds.size.width;
    
    if (engine != NULL) {
        if (isPointInCircle(Vector2D<float>(x, y), Vector2D<float>(engine->controlBasePos), JOYSTICK_RADIUS)){
            isPressedPos = true;
        }
        
        if (isPressedPos) {
            //DPAD
            float dx = x - engine->controlBasePos.x;
            float dy = y - engine->controlBasePos.y;
            float angle = atan2(dy, dx); // in radians
            
            float anglePerSector = 360.0f / 4 * (M_PI / 180.0f);
            angle = roundf(angle / anglePerSector) * anglePerSector;
            
            Vector2D<float> velocity = Vector2D<float>(cos(angle), sin(angle));
            
            engine->controlKnobPos.x = engine->controlBasePos.x + cos(angle) * THUMB_RADIUS;
            engine->controlKnobPos.y = engine->controlBasePos.y + sin(angle) * THUMB_RADIUS;
            
            touchDirection = 0;
            if (((int)velocity.x) == -1) {
                touchDirection |= Engine::DIRECTION::LEFT;
            }
            else if (((int)velocity.y) == 1) {
                touchDirection |= Engine::DIRECTION::DOWN;
            }
            else if (((int)velocity.x) == 1) {
                touchDirection |= Engine::DIRECTION::RIGHT;
            }
            else if (((int)velocity.y) == -1) {
                touchDirection |= Engine::DIRECTION::UP;
            }
        }
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesEnded:touches withEvent:event];
    isPressedPos=false;
    engine->controlKnobPos = engine->controlBasePos;
    touch = false;
    touchDirection=0;
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    [super touchesCancelled:touches withEvent:event];
}
- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    engine->handleInput(1, touchDirection);
    engine->update(1/60);
	engine->render(1/60);
}
@end
