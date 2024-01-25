#include <string.h>
#include <tdk.h>

void bottombar(void);
void desktop(void);
void die(char *msg);
void topbar(void);
void win(void);

int wcol;
int wln;

void
bottombar(void)
{
	tdk_setcpos(wcol - 18, wln - 2);
	printf("  skippyr/libtdk");
	tdk_setcpos(wcol - 30, wln - 1);
	tdk_setclr(tdk_ClrRed, tdk_LyrFg);
	tdk_seteff(tdk_EffUnd, 1);
	printf("www.github.com/skippyr/libtdk");
	tdk_seteff(tdk_EffUnd, 0);
	tdk_setclr(tdk_ClrDft, tdk_LyrFg);
	tdk_setcpos(0, wln - 1);
	printf("  Press ");
	tdk_setlum(tdk_LumBld);
	printf("Escape");
	tdk_setlum(tdk_LumDft);
	printf(" or ");
	tdk_setlum(tdk_LumBld);
	printf("Q");
	tdk_setlum(tdk_LumDft);
	printf(" to exit preview.");
}

void
desktop(void)
{
	tdk_setcpos(0, 2);
	printf("     \n");
	printf("  Trash\n\n");
	printf("     \n");
	printf(" Firefox\n\n");
	printf("     \n");
	printf(" Help.pdf\n\n");
}

void
die(char *msg)
{
	tdk_setclr(tdk_ClrRed, tdk_LyrFg);
	fprintf(stderr, "preview: ");
	tdk_setclr(tdk_ClrDft, tdk_LyrFg);
	fprintf(stderr, "%s", msg);
	exit(1);
}

void
topbar(void)
{
	char *opts[] = {"File", "View", "Options", "Help"};
	char stats[] = "󰃭 (Wed) Jan 24th  󰽥 19h00m   Search";
	int i;
	tdk_seteff(tdk_EffRev, 1);
	for (i = 0; i < wcol; i++)
		printf(" ");
	tdk_setcpos(wcol - 37, 0);
	printf("%s", stats);
	tdk_setcpos(0, 0);
	tdk_setlum(tdk_LumBld);
	printf("   libtdk - preview ");
	tdk_setlum(tdk_LumDft);
	for (i = 0; i < sizeof(opts) / sizeof(NULL); i++)
		printf(" %s ", opts[i]);
	tdk_seteff(tdk_EffRev, 0);
}

void
win(void)
{
	tdk_setcpos(wcol - 30, 5);
	tdk_seteff(tdk_EffRev, 1);
	printf(" ┌ 󰀦  Alert ─────[x]─┐ ");
	tdk_setcpos(wcol - 30, 6);
	printf(" │ Here Are Dragons! │ ");
	tdk_setcpos(wcol - 30, 7);
	printf(" └───────────────────┘ ");
	tdk_seteff(tdk_EffRev, 0);
}

int
main(void)
{
	int key;
	tdk_getwdim(&wcol, &wln);
	if (!isatty(1) || !isatty(2))
		die("no output stream must be redirected.\n");
	if (wcol < 83)
		die("must have, at least, 83 columns.\n");
	tdk_setwalt(1);
	tdk_setcvis(0);
	topbar();
	bottombar();
	desktop();
	win();
	while ((key = tdk_getkey()) != tdk_KeyEsc && key != 'q' && key != 'Q');
	tdk_setcvis(1);
	tdk_setwalt(0);
	return 0;
}