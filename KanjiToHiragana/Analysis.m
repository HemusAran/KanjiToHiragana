#import "Analysis.h"

#include "Dictionary.h"
#include "LanguageAnalysis.h"

@implementation Analysis


char* strsub(char *string, int start, size_t nbyte) {
    char *str;
    
    if(nbyte == 0) {
        nbyte = strlen(string);
    }
    str = (char*)calloc((nbyte+1), sizeof(char));
    
    string += start;
    strncpy(str, string, nbyte);
    
    return str;
}

/*
 UTF8 の NSString にして返す
 */
+ (NSString *)stringToUTF8String : (NSString *)str
{
	NSData *data = [ str dataUsingEncoding : NSUTF8StringEncoding ];
	return [[NSString alloc] initWithData : data
								 encoding : NSUTF8StringEncoding];
}



/*
	メインメソッド
	漢字かな混じりの文字列をひらがなに変換して返す。
	文字コードはUTF-8で行うこと。
	（このメソッドの作りがUTF-8前提。LATextToMorphemesはたぶん違う）
 */
+ (NSString *)analysis : (NSString *)analysisString
{
	OSStatus err;
	LAContextRef context;	// 解析に使用するコンテキスト
	LAEnvironmentRef environment;
	Str63 environmentName = kLAJapaneseTTSEnvironment;
			// kLAJapaneseTTSEnvironment : 東京 -> とーきょー
			// kLAJapaneseMorphemeAnalysisEnvironment : 東京 -> とうきょう
			// kLAJapaneseKanaKanjiEnvironment : とうきょう -> 東京

	NSString *result = nil;

	@try {
		err = LAGetEnvironmentRef(environmentName, &environment);
		if (err != noErr) {
			fprintf(stderr, "LAGetEnvironmentRef error\n");
			@throw [NSException exceptionWithName:nil reason:nil userInfo:nil];
		}
		
		err = LAOpenAnalysisContext(environment, &context);
		if (err != noErr) {
			fprintf(stderr, "LAOpenAnalysisContext error\n");
			@throw [NSException exceptionWithName:nil reason:nil userInfo:nil];
		}
		
		
		TextEncoding preferedEncoding = kTextEncodingMacJapanese;	// 入出力に使用するテキストのエンコード
		ByteCount textLength;	// 解析するテキストの長さ
		char *sourceText;
		ByteCount bufferSize;	// 結果を得るための resultBuffer のサイズ
		OptionBits convertFlags;	// 解析をコントロールするフラグ
		UInt32 structureVersion = kLAMorphemesArrayVersion;	// 現在の LAMOrphemesArrayPtr のバージョン
		ByteCount *acceptedLength;	// エンジンに受け取られたテキストの長さ
		LAMorphemesArrayPtr resultBuffer;	// 結果を受け取るバッファ
		
		// 解析文字列をsjisに変換
		sourceText = (char *)[ analysisString cStringUsingEncoding : NSShiftJISStringEncoding ];
		textLength = strlen(sourceText);
		convertFlags = kLAEndOfSourceTextMask;	// sourceTextが最後までデータが入ってることを示す
		
		char buffer[1024];
		resultBuffer = (LAMorphemesArrayPtr)buffer;
		bufferSize = sizeof(buffer);
		resultBuffer->morphemesCount = 0;
		
		// 解析データを蓄積するオブジェクト
		NSMutableString *mutable = [ NSMutableString stringWithString : @"" ];
		
		while (textLength || resultBuffer->morphemesCount ) {
			err = LATextToMorphemes( context
									, preferedEncoding
									, textLength
									, sourceText
									, bufferSize
									, convertFlags
									, structureVersion
									, &acceptedLength
									, resultBuffer
									);
			
			if (err != noErr) {
				if ([analysisString length] == 1) {	// 平仮名1文字の場合にエラーになるので回避
					result = analysisString;
				} else {
					fprintf(stderr, "LATextToMorphemes error\n");
				}
				@throw [NSException exceptionWithName:nil reason:nil userInfo:nil];
			}
			
			if ( resultBuffer->morphemesCount > 0 ) {
				// 最後にゴミが入らないようstrsubで文字列を取得
				// 解析途中のデータをmutableに蓄積
				[ mutable appendString : [NSString stringWithCString :
										  strsub(resultBuffer->morphemes[0].morphemeTextPtr, 0, resultBuffer->morphemesTextLength)
															encoding : NSShiftJISStringEncoding]];
			}

			// 解析文章の位置、解析文字数を変更
			sourceText += (unsigned long)acceptedLength;
			textLength -= (unsigned long)acceptedLength;
		}
		
		// 蓄積された解析データをUTF8に戻す
		result = [ Analysis stringToUTF8String : mutable ];
		
	} @catch (NSException *e) {
		// none
	} @finally {
		
		LAResetAnalysis(context);
		LACloseAnalysisContext(context);

		return result;
	}
	
	return nil;
}


@end
