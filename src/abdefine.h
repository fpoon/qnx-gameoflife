/* Define header for application - AppBuilder 2.03  */

/* 'base' Window link */
extern const int ABN_base;
#define ABW_base                             AbGetABW( ABN_base )
extern const int ABN_prDrawer;
#define ABW_prDrawer                         AbGetABW( ABN_prDrawer )
extern const int ABN_pbStart;
#define ABW_pbStart                          AbGetABW( ABN_pbStart )
extern const int ABN_pbStep;
#define ABW_pbStep                           AbGetABW( ABN_pbStep )
extern const int ABN_pbStop;
#define ABW_pbStop                           AbGetABW( ABN_pbStop )
extern const int ABN_pbReboot;
#define ABW_pbReboot                         AbGetABW( ABN_pbReboot )
extern const int ABN_pbSpeedLess;
#define ABW_pbSpeedLess                      AbGetABW( ABN_pbSpeedLess )
extern const int ABN_pbSpeedMore;
#define ABW_pbSpeedMore                      AbGetABW( ABN_pbSpeedMore )
extern const int ABN_pbDraw;
#define ABW_pbDraw                           AbGetABW( ABN_pbDraw )
extern const int ABN_pbMove;
#define ABW_pbMove                           AbGetABW( ABN_pbMove )
extern const int ABN_pbZoomIn;
#define ABW_pbZoomIn                         AbGetABW( ABN_pbZoomIn )
extern const int ABN_pbZoomOut;
#define ABW_pbZoomOut                        AbGetABW( ABN_pbZoomOut )
extern const int ABN_psFillSlider;
#define ABW_psFillSlider                     AbGetABW( ABN_psFillSlider )
extern const int ABN_pbFill;
#define ABW_pbFill                           AbGetABW( ABN_pbFill )
extern const int ABN_plFillLabel;
#define ABW_plFillLabel                      AbGetABW( ABN_plFillLabel )
extern const int ABN_pcbColors;
#define ABW_pcbColors                        AbGetABW( ABN_pcbColors )
extern const int ABN_pbNew;
#define ABW_pbNew                            AbGetABW( ABN_pbNew )
extern const int ABN_pbOpen;
#define ABW_pbOpen                           AbGetABW( ABN_pbOpen )
extern const int ABN_pbSave;
#define ABW_pbSave                           AbGetABW( ABN_pbSave )
extern const int ABN_pbCyclic;
#define ABW_pbCyclic                         AbGetABW( ABN_pbCyclic )
extern const int ABN_pbMulti;
#define ABW_pbMulti                          AbGetABW( ABN_pbMulti )
extern const int ABN_ptRules;
#define ABW_ptRules                          AbGetABW( ABN_ptRules )
extern const int ABN_ptWidth;
#define ABW_ptWidth                          AbGetABW( ABN_ptWidth )
extern const int ABN_ptHeight;
#define ABW_ptHeight                         AbGetABW( ABN_ptHeight )
extern const int ABN_pbResize;
#define ABW_pbResize                         AbGetABW( ABN_pbResize )
extern const int ABN_plPopStep;
#define ABW_plPopStep                        AbGetABW( ABN_plPopStep )
extern const int ABN_plMinMax;
#define ABW_plMinMax                         AbGetABW( ABN_plMinMax )

#define AbGetABW( n ) ( AbWidgets[ n ].wgt )

#define AB_OPTIONS "s:x:y:h:w:S:"
