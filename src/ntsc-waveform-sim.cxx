#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/fl_draw.H>
#include <stdio.h>
#include <math.h>

// NTSC Waveform Monitor Simulation
//     ..with some NTSC test patterns
//
// erco 04/17/2005
//

#define GREEN     0xb0			// brightness
#define GREENBURN (0xff-GREEN)		// burn brightness

// NTSC color bar scope simulator
class Scope : public Fl_Group {
    char stop;				// 'stop drawing' flag
    float xzoom, yzoom, xoff, yoff;	// zoom, x/y offset
    float save[4];			// saved zoom, x/y offset
    unsigned int lcolor;		// line color
    char testpattern;			// 0=black, 1=colorbars..
    float beamx, beamy;			// current beam position

    // DRAW A LINE BETWEEN TWO POINTS, AFFECTED BY ZOOM/OFFSET
    void Line(float x1,float y1,float x2, float y2) {
        x1 = (x1 * xzoom) + xoff;
        x2 = (x2 * xzoom) + xoff;
        y1 = (y1 * yzoom) + yoff; y1 = y()+h() - y1;	// inverted y
        y2 = (y2 * yzoom) + yoff; y2 = y()+h() - y2;	// inverted y
	// BURN HACK: FORCE HORIZ LINES FULL BRIGHTNESS
	if (y1==y2) fl_color((GREEN+GREENBURN)<<16);	// full brightness for horiz line
	else        fl_color(lcolor);
	fl_line((int)x1,(int)y1,(int)x2,(int)y2);
	fl_line((int)x1+1,(int)y1+1,(int)x2+1,(int)y2+1);
	if ( x2 > x()+w() ) stop = 1;   // tells draw() to stop drawing, we hit the right edge of the scope
    }
    // TRACE RELATIVE TO CURRENT POSITION
    void TraceRel(float xo, float yo) {
        Line(beamx,beamy,beamx+xo,beamy+yo);
	beamx=beamx+xo; beamy=beamy+yo;
    }
    // TRACE RELATIVE TO ABSOLUTE POSITION
    void TraceTo(float x2, float y2) {
        Line(beamx,beamy,x2,y2);
	beamx=x2; beamy=y2;
    }
    // TRACE OUT A SINE WAVE
    void SinCycle(float amp, float usecs, int cycles) {
        int samps = 15;
        float ysave = beamy;		// save ypos
        float xsave = beamx;		// save xpos
	unsigned int csave = lcolor;	// save line color
	float cw = (usecs / cycles);	// cycle width
	for ( int cycle=0; cycle<cycles; cycle++ ) {
	    // BURN HACK: TOPS AND BOTTOMS OF SIN WAVE BRIGHTER
	    //     Simulates analog phosphor burn as trace slows
	    //
	    float st;
	    unsigned int burn;
	    for ( float t=1; t<=samps; t++ )	// #samples in sin wave
	    {
	        st = sin((t/samps) * 6.28);
		burn = (unsigned int)(fabs(st) * GREENBURN);	// top/bottoms bright
		lcolor = (GREEN << 16) + (burn << 16);
	        TraceTo(xsave+(cycle*cw)+((t/samps)*cw),ysave+sin((t/samps)*6.28)*amp);
	    }
	    beamy = ysave;			// anti-drift
	}
	// return to default line color
	lcolor = csave;
    }
    void BlackPattern() {
	TraceRel(0,7.5);			// rise to 7.5 IRE (black)
	TraceRel(52.6,0);			// 52.6 uSecs of black
	TraceRel(0,-7.5);			// back down to porch
    }
    // SMPTE 75% IRE COLOR BARS WITH 100% WHITE BAR
    void ColorBarsSMPTEPattern() {
	// X values in uSecs, Y values in IRE

	// 52.6us of video colorbars
	//    took guesses at the chroma amplitudes..
	//
	float bw = 52.6 / 8.0;	// each bar's width in uSecs
	int cycles = (int)(bw * 3.58);
	TraceTo(beamx,40+100);	// jump to 100 IRE (white)
	TraceRel(bw,0);		// white
	TraceRel(0,-11);	// down to 89 IRE (yel)
	SinCycle(15,bw,cycles);	// yel
	TraceRel(0,-19);	// down to 70 IRE (cyan)
	SinCycle(35,bw,cycles);	// cyan
	TraceRel(0,-11);	// down to 59 IRE (grn)
	SinCycle(40,bw,cycles);	// grn
	TraceRel(0,-18);	// down to 41 IRE (mag)
	SinCycle(50,bw,cycles);	// mag
	TraceRel(0,-11);	// down to 30 IRE (red)
	SinCycle(55,bw,cycles);	// red
	TraceRel(0,-19);	// down to 11 IRE (blu)
	SinCycle(40,bw,cycles);	// blu
	TraceRel(0,-11);	// down to 0 IRE (blk)
	TraceRel(bw,0);		// blk
    }
    // MULTIBURST
    void MultiburstPattern() {
	// X values in uSecs, Y values in IRE
	float bwid = 52.6 / 7;
	float bsep = bwid / 7;

	// Burst#1: 1 cycle square wave from 70 ~ 10 IRE
	TraceTo(beamx, 40+70);		// to 70 IRE
	TraceRel((bwid-bsep)/2, 0);	// hold
	TraceTo(beamx, 40+10);		// to 10 IRE
	TraceRel((bwid-bsep)/2, 0);	// hold
	TraceTo(beamx, 40+40);		// to 40 IRE
	TraceRel(bsep, 0);		// hold for sep

	// Burst #2
	SinCycle(30, (bwid-bsep), 3);
	TraceRel(bsep, 0);		// hold for sep

	// Burst #3
	SinCycle(30, (bwid-bsep), 6);
	TraceRel(bsep, 0);		// hold for sep

	// Burst #4
	SinCycle(30, (bwid-bsep), 9);
	TraceRel(bsep, 0);		// hold for sep

	// Burst #5
	SinCycle(30, (bwid-bsep), 12);
	TraceRel(bsep, 0);		// hold for sep

	// Burst #6
	SinCycle(30, (bwid-bsep), 16);
	TraceRel(bsep, 0);		// hold for sep

	// Burst #7
	SinCycle(30, (bwid-bsep), 19);
	TraceRel(bsep, 0);		// hold for sep

	// TraceTo(xsave+52.6, 40+7.5);	// DEBUG: ramp down
    }
    // DRAW AN NTSC VIDEO SIGNAL
    //     Just one video scanline is repeated indefinitely.
    //     No vertical sync.
    //     X values in uSecs, Y values in IRE.
    //
    void DrawSignal() {
	// Default line color
	lcolor = GREEN << 16;	// ff -> 00ff0000

	// Hsync
        TraceRel(4.7, 0);	// 4.7us of hsync
        TraceRel(0.1,40);	// rising edge to back porch w/analog edge

	// Back porch
        TraceRel(1.1, 0);  	// back porch head
        SinCycle(20,2.5,8);	// 8 cycles of colorburst in 2.5us
        TraceRel(1.1, 0);	// back porch tail

	// Video signal..
	switch ( testpattern ) {
	    case 0: BlackPattern(); break; 
	    case 1: ColorBarsSMPTEPattern(); break;
	    case 2: MultiburstPattern(); break;
	}

	// Front porch
	TraceTo(beamx,40);	// jump to 0 IRE (black)
	TraceRel(1.5,0);	// 1.5us of front porch
        TraceRel(0.1,-40);	// drop from front porch w/analog edge
    }
    // DRAW THE SCOPE'S NON-SCALING GRATICULE
    void DrawGrid() {
        int x1 = x(), x2 = x()+w()-1,
	    y1 = y()+h()-1, y2 = y();
	for ( int X=x1; X<x2; X+=50 ) fl_line(X,y1,X,y2);
	for ( int Y=y1; Y>y2; Y-=40 ) fl_line(x1,Y,x2,Y);
    }
    // FLTK calls this to draw the Scope widget.
    //     This is the top level for handling drawing..
    //
    void draw() {
        Fl_Group::draw();
	fl_push_clip(x(),y(),w(),h());
	{
	    beamx=0;
	    beamy=0;
	    for ( stop=0; !stop; )	// repeat signal until we hit right edge of screen
		DrawSignal();
	    fl_color(0x50505000);
	    DrawGrid();
	}
	fl_pop_clip();
    }
public:
    Scope(int X, int Y, int W, int H, const char *L=0):Fl_Group(X,Y,W,H,L) {
        box(FL_BORDER_BOX);
	color(40);
	xzoom = 1;
	yzoom = 1;
	xoff=0;
	yoff=0;
	end();
    }
    void  SetXZoom(float val) { xzoom = val; redraw(); }
    float GetXZoom()          { return(xzoom); }
    void  SetYZoom(float val) { yzoom = val; redraw(); }
    float GetYZoom()          { return(yzoom); }

    void  SetXOffset(float val) { xoff = val; redraw(); }
    float GetXOffset()          { return(xoff); }
    void  SetYOffset(float val) { yoff = val; redraw(); }
    float GetYOffset()          { return(yoff); }

    void Save() { save[0] = xzoom; save[1] = yzoom; save[2] = xoff; save[3] = yoff; }
    void Load() { xzoom = save[0]; yzoom = save[1]; xoff = save[2]; yoff = save[3]; redraw(); }

    void SetTestPattern(char flag) { testpattern = flag; redraw(); }
};

Scope *G_scope = 0;
Fl_Value_Slider *G_xzoom = 0;
Fl_Value_Slider *G_yzoom = 0;
Fl_Value_Slider *G_xoffset = 0;
Fl_Value_Slider *G_yoffset = 0;

void TestPattern_CB(Fl_Widget*w, void*d)
    { //G_scope->SetTestPattern((char)d); 
    }
void XZoom_CB(Fl_Widget*w, void*)
    { G_scope->SetXZoom(((Fl_Slider*)w)->value()); }
void YZoom_CB(Fl_Widget*w, void*)
    { G_scope->SetYZoom(((Fl_Slider*)w)->value()); }
void XOffset_CB(Fl_Widget*w, void*)
    { G_scope->SetXOffset(((Fl_Slider*)w)->value()); }
void YOffset_CB(Fl_Widget*w, void*)
    { G_scope->SetYOffset(((Fl_Slider*)w)->value()); }
void Finder_CB(Fl_Widget*w, void*)
{
    // Reload settings, update all sliders
    G_scope->Load();
    G_xzoom->value(G_scope->GetXZoom());
    G_yzoom->value(G_scope->GetYZoom());
    G_xoffset->value(G_scope->GetXOffset());
    G_yoffset->value(G_scope->GetYOffset());
}
int main() {
    Fl::visual(FL_RGB);
    Fl_Double_Window win(600,520,"oscilloscope");

    G_scope = new Scope(10,10,600-20,380);
    G_scope->SetXZoom(5.00);
    G_scope->SetYZoom(2.00);
    G_scope->SetXOffset(0.00);
    G_scope->SetYOffset(40);
    G_scope->Save();

    G_xzoom = new Fl_Value_Slider(10,400,225,20,"X Zoom");
    G_xzoom->callback(XZoom_CB);
    G_xzoom->type(FL_HOR_NICE_SLIDER);
    G_xzoom->range(0.5, 200.0);

    G_yzoom = new Fl_Value_Slider(10,440,225,20,"Y Zoom");
    G_yzoom->callback(YZoom_CB);
    G_yzoom->type(FL_HOR_NICE_SLIDER);
    G_yzoom->range(0.1, 10.0);

    G_xoffset = new Fl_Value_Slider(250,400,225,20,"X Offset");
    G_xoffset->callback(XOffset_CB);
    G_xoffset->type(FL_HOR_NICE_SLIDER);
    G_xoffset->range(-10000, 100.0);

    G_yoffset = new Fl_Value_Slider(250,440,225,20,"Y Offset");
    G_yoffset->callback(YOffset_CB);
    G_yoffset->type(FL_HOR_NICE_SLIDER);
    G_yoffset->range(-1000, 100.0);

    Fl_Button finder(500, 400, 90, 25, "Beam Find");
    finder.callback(Finder_CB);
    Finder_CB(0,0);		// sync sliders to scope's current values

    int defaultpattern = 1;
    Fl_Choice choice(100, 480, 140, 25, "Pattern");
    choice.add("Black",      0, TestPattern_CB, (void*)0);
    choice.add("SMPTE Bars", 0, TestPattern_CB, (void*)1);
    choice.add("Multiburst", 0, TestPattern_CB, (void*)2);
    choice.value(defaultpattern);
    G_scope->SetTestPattern(defaultpattern);

    win.end();
    win.resizable(win);
    win.show();
    return(Fl::run());
}