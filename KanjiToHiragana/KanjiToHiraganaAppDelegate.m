#import "KanjiToHiraganaAppDelegate.h"

#import "Analysis.h"


@implementation KanjiToHiraganaAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	// 漢字かな混じり文字をひらがなに変換してログ出力
	NSString *str = @"漢字かな混じり文字をひらがなに変換してログ出力";
	
	NSLog([Analysis analysis : str]);
}

@end
