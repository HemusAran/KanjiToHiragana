/* 	File:		LanguageAnalysis.h  	Contains:	Interfaces to Analysis Manager   	Version:	Technology: Language Analysis Architecture 1.1.1				Release: To be used with Universal Interfaces 3.2  	Copyright:	1998 by Apple Computer, Inc., all rights reserved  	Bugs?:		Please include the the file and version information (from above) with 				the problem description.  Developers belonging to one of the Apple 				developer programs can submit bug reports to:  					daruma@apple.com */#ifndef __LANGUAGEANALYSIS__#define __LANGUAGEANALYSIS__#ifndef __TYPES__#include <Types.h>#endif#ifndef __FILES__#include <Files.h>#endif#ifndef __APPLEEVENTS__#include <AppleEvents.h>#endif#ifndef __AEREGISTRY__#include <AERegistry.h>#endif#ifndef __DICTIONARY__#include <Dictionary.h>#endif#ifndef __TEXTCOMMON__#include <TextCommon.h>#endif#ifdef __cplusplusextern "C" {#endif#if PRAGMA_IMPORT_SUPPORTED#pragma import on#endif#if PRAGMA_ALIGN_SUPPORTED#pragma options align=powertypedef struct OpaqueLAEnvironmentRef* LAEnvironmentRef;typedef struct OpaqueLAContextRef* LAContextRef;typedef AEKeyword LAPropertyKey;typedef DescType LAPropertyType;/*	Data structure for high level API*/struct LAMorphemeRec{	ByteCount			sourceTextLength;	LogicalAddress		sourceTextPtr;	ByteCount			morphemeTextLength;	LogicalAddress		morphemeTextPtr;	UInt32				partOfSpeech;};typedef struct LAMorphemeRec LAMorphemeRec;struct LAMorphemesArray{	ItemCount			morphemesCount;	ByteCount			processedTextLength;	ByteCount			morphemesTextLength;	LAMorphemeRec		morphemes[kVariableLengthArray];};typedef struct LAMorphemesArray LAMorphemesArray, *LAMorphemesArrayPtr;enum {	kLAMorphemesArrayVersion	= 0};enum {    kLAEndOfSourceTextMask  = 0x00000001}; /*	Definitions for result path/bundle structure*/typedef AERecord LAMorphemeBundle;typedef AERecord LAMorphemePath;typedef AERecord LAMorpheme;typedef AERecord LAHomograph;enum {	keyAELAMorphemeBundle		= 'lmfb',	keyAELAMorphemePath			= 'lmfp',	keyAELAMorpheme				= 'lmfn',	keyAELAHomograph			= 'lmfh'};enum {	typeLAMorphemeBundle		= typeAERecord,	typeLAMorphemePath			= typeAERecord,	typeLAMorpheme				= typeAEList,	typeLAHomograph				= typeAEList};/*	Definitions for morpheme/homograph information*/enum {	keyAEMorphemePartOfSpeechCode	= 'lamc',	keyAEMorphemeTextRange			= 'lamt'};enum {	typeAEMorphemePartOfSpeechCode	= 'lamc',	typeAEMorphemeTextRange			= 'lamt'};typedef UInt32		MorphemePartOfSpeech;struct MorphemeTextRange{	UInt32			sourceOffset;	UInt32			length;};typedef struct MorphemeTextRange MorphemeTextRange;/*	Constants for leading/trailing path of analysis function*/enum {	kLADefaultEdge				= 0,	kLAFreeEdge					= 1,	kLAIncompleteEdge			= 2};/*	Constants for confirm and shift function*/enum {	kLAAllMorphemes				= 0};/*	Error Value*/enum {	laEngineNotFoundErr			= -7000,						/* can't find the engine*/	laPropertyErr				= -6999,						/* Error in properties*/	laPropertyNotFoundErr		= -6998,						/* can't find the property*/	laPropertyIsReadOnlyErr		= -6997,						/* the property is read only*/	laPropertyUnknownErr		= -6996,						/* the property is unknown to this environment*/	laPropertyValueErr			= -6995,						/* Invalid property value*/	laDictionaryTooManyErr		= -6994,						/* too many dictionaries*/	laDictionaryUnknownErr		= -6993,						/* can't use this dictionary with this environment*/	laDictionaryNotOpenedErr	= -6992,						/* the dictionary is not opened*/	laTextOverFlowErr			= -6991,						/* text is too long*/	laFailAnalysisErr			= -6990,						/* analysis failed*/	laNoMoreMorphemeErr			= -6989,						/* nothing to read*/	laInvalidPathErr			= -6988,						/* path is not correct */	laEnvironmentExistErr		= -6987,						/* same name environment is already exists */	laEnvironmentNotFoundErr	= -6986,						/* can't fint the specified environment */	laEnvironmentBusyErr		= -6985,						/* specified environment is used */    laTooSmallBufferErr			= -6984   						/* output buffer is too small to store any result */ };/*	Library version*/extern pascal UInt32 LALibraryVersion(void);/*	High level API*/extern pascal OSStatus LATextToMorphemes(LAContextRef context, TextEncoding preferedEncoding, ByteCount textLength, ConstLogicalAddress sourceText, ByteCount bufferSize, OptionBits convertFlags, UInt32 structureVersion, ByteCount *acceptedLength, LAMorphemesArrayPtr resultBuffer);/*	Handling Context*/extern pascal OSStatus LAOpenAnalysisContext(LAEnvironmentRef environ, LAContextRef *context);extern pascal OSStatus LACloseAnalysisContext(LAContextRef context);/*	Handling Environment*/extern pascal OSStatus LAGetEnvironmentList( UInt32 maxCount, UInt32 *actualCount, LAEnvironmentRef environmentList[]);extern pascal OSStatus LAGetEnvironmentName( LAEnvironmentRef environment, Str63 environmentName);extern pascal OSStatus LAGetEnvironmentRef( ConstStr63Param targetEnvironmentName, LAEnvironmentRef *environment);extern pascal OSStatus LACreateCustomEnvironment( LAEnvironmentRef baseEnvironment, ConstStr63Param newEnvironmentName, Boolean persistent, LAEnvironmentRef *newEnvironment);extern pascal OSStatus LADeleteCustomEnvironment( LAEnvironmentRef environment);/*	Handling dictionries*/extern pascal OSStatus LAOpenDictionary(LAEnvironmentRef environ, const FSSpec *dictionary);extern pascal OSStatus LACloseDictionary(LAEnvironmentRef environ, const FSSpec *dictionary);extern pascal OSStatus LAListAvailableDictionaries(LAEnvironmentRef environ, ItemCount maxCount, ItemCount *actualCount, FSSpec dictionaryList[], Boolean opened[]);/*	Analyzing text*/extern pascal OSStatus LAMorphemeAnalysis(LAContextRef context, ConstUniCharArrayPtr text, UniCharCount textLength, LAMorphemePath *leadingPath, LAMorphemePath *trailingPath, ItemCount pathCount, LAMorphemeBundle *result);extern pascal OSStatus LAContinuousMorphemeAnalysis(LAContextRef context, ConstUniCharArrayPtr text, UniCharCount textLength, Boolean incrementalText, LAMorphemePath *leadingPath, LAMorphemePath *trailingPath, Boolean *modified);extern pascal OSStatus LAGetMorphemes(LAContextRef context, LAMorphemePath *result);extern pascal OSStatus LAShiftMorphemes(LAContextRef context, ItemCount morphemeCount, LAMorphemePath *path, UniCharCount *shiftedLength);extern pascal OSStatus LAResetAnalysis(LAContextRef context);/*	Check Language Analysis Manager availability*/#ifdef __cplusplusinline pascal Boolean LALanguageAnalysisAvailable() { return (LALibraryVersion != (void*)kUnresolvedCFragSymbolAddress); }#else#define LALanguageAnalysisAvailable() 	(LALibraryVersion != (void*)kUnresolvedCFragSymbolAddress)#endif#pragma options align=reset#endif /* PRAGMA_ALIGN_SUPPORTED */#if PRAGMA_IMPORT_SUPPORTED#pragma import off#endif#ifdef __cplusplus}#endif#endif /* __LANGUAGEANALYSIS__ */