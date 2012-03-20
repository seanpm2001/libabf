#ifndef ABF2_STRUCT_H
#define ABF2_STRUCT_H

#include <stdint.h>

typedef int8_t t_BOOL;

/* FALSE = 0, TRUE = 1 */
typedef enum { FALSE, TRUE } Boolean;
#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct guid
{
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t Data4[8];
} GUID;
#endif /* GUID_DEFINED */

// Structure definitions for Axon Binary File v2 (ABF2) format
//
// Code was taken from ABFFIO/ProtocolStructs.h and modified from C++ to pure C

#define ABF2_FILESIGNATURE   0x32464241      /* "ABF2" in little-endian ASCII */
                                             /* "2FBA" in big-endian ASCII */

struct abf2_section
{
    uint32_t uBlockIndex;        // ABF block number of the first entry
    uint32_t uBytes;             // size in bytes of of each entry
    int64_t llNumEntries;        // number of entries in this section
};

struct abf2_fileinfo
{
    uint32_t uFileSignature;
    uint32_t uFileVersionNumber;

    uint32_t uFileInfoSize;
    uint32_t uActualEpisodes;
    uint32_t uFileStartDate;
    uint32_t uFileStartTimeMS;
    uint32_t uStopwatchTime;
    int16_t nFileType;
    int16_t nDataFormat;
    int16_t nSimultaneousScan;
    int16_t nCRCEnable;
    uint32_t uFileCRC;
    struct guid FileGUID;
    uint32_t uCreatorVersion;
    uint32_t uCreatorNameIndex;
    uint32_t uModifierVersion;
    uint32_t uModifierNameIndex;
    uint32_t uProtocolPathIndex;

    // New sections in ABF 2 - protocol stuff ...
    struct abf2_section ProtocolSection;           // the protocol
    struct abf2_section ADCSection;                // one for each ADC channel
    struct abf2_section DACSection;                // one for each DAC channel
    struct abf2_section EpochSection;              // one for each epoch
    struct abf2_section ADCPerDACSection;          // one for each ADC for each DAC
    struct abf2_section EpochPerDACSection;        // one for each epoch for each DAC
    struct abf2_section UserListSection;           // one for each user list
    struct abf2_section StatsRegionSection;        // one for each stats region
    struct abf2_section MathSection;
    struct abf2_section StringsSection;

    // ABF 1 sections ...
    struct abf2_section DataSection;            // Data
    struct abf2_section TagSection;             // Tags
    struct abf2_section ScopeSection;           // Scope config
    struct abf2_section DeltaSection;           // Deltas
    struct abf2_section VoiceTagSection;        // Voice Tags
    struct abf2_section SynchArraySection;      // Synch Array
    struct abf2_section AnnotationSection;      // Annotations
    struct abf2_section StatsSection;           // Stats config

    int8_t sUnused[148];     // size = 512 bytes
};

struct abf2_protocolinfo
{
    int16_t nOperationMode;
    float fADCSequenceInterval;
    t_BOOL bEnableFileCompression;
    int8_t sUnused1[3];
    uint32_t uFileCompressionRatio;

    float fSynchTimeUnit;
    float fSecondsPerRun;
    int32_t lNumSamplesPerEpisode;
    int32_t lPreTriggerSamples;
    int32_t lEpisodesPerRun;
    int32_t lRunsPerTrial;
    int32_t lNumberOfTrials;
    int16_t nAveragingMode;
    int16_t nUndoRunCount;
    int16_t nFirstEpisodeInRun;
    float fTriggerThreshold;
    int16_t nTriggerSource;
    int16_t nTriggerAction;
    int16_t nTriggerPolarity;
    float fScopeOutputInterval;
    float fEpisodeStartToStart;
    float fRunStartToStart;
    int32_t  lAverageCount;
    float fTrialStartToStart;
    int16_t nAutoTriggerStrategy;
    float fFirstRunDelayS;

    int16_t nChannelStatsStrategy;
    int32_t lSamplesPerTrace;
    int32_t lStartDisplayNum;
    int32_t lFinishDisplayNum;
    int16_t nShowPNRawData;
    float fStatisticsPeriod;
    int32_t lStatisticsMeasurements;
    int16_t nStatisticsSaveStrategy;

    float fADCRange;
    float fDACRange;
    int32_t lADCResolution;
    int32_t lDACResolution;

    int16_t nExperimentType;
    int16_t nManualInfoStrategy;
    int16_t nCommentsEnable;
    int32_t lFileCommentIndex;
    int16_t nAutoAnalyseEnable;
    int16_t nSignalType;

    int16_t nDigitalEnable;
    int16_t nActiveDACChannel;
    int16_t nDigitalHolding;
    int16_t nDigitalInterEpisode;
    int16_t nDigitalDACChannel;
    int16_t nDigitalTrainActiveLogic;

    int16_t nStatsEnable;
    int16_t nStatisticsClearStrategy;

    int16_t nLevelHysteresis;
    int32_t lTimeHysteresis;
    int16_t nAllowExternalTags;
    int16_t nAverageAlgorithm;
    float fAverageWeighting;
    int16_t nUndoPromptStrategy;
    int16_t nTrialTriggerSource;
    int16_t nStatisticsDisplayStrategy;
    int16_t nExternalTagType;
    int16_t nScopeTriggerOut;

    int16_t nLTPType;
    int16_t nAlternateDACOutputState;
    int16_t nAlternateDigitalOutputState;

    float fCellID[3];

    int16_t nDigitizerADCs;
    int16_t nDigitizerDACs;
    int16_t nDigitizerTotalDigitalOuts;
    int16_t nDigitizerSynchDigitalOuts;
    int16_t nDigitizerType;

    int8_t sUnused[304];     // size = 512 bytes
};

struct abf2_mathinfo
{
    int16_t nMathEnable;
    int16_t nMathExpression;
    uint32_t uMathOperatorIndex;
    uint32_t uMathUnitsIndex;
    float fMathUpperLimit;
    float fMathLowerLimit;
    int16_t nMathADCNum[2];
    int8_t  sUnused[16];
    float fMathK[6];

    int8_t sUnused2[64];     // size = 128 bytes
};

struct abf2_adcinfo
{
    // The ADC this struct is describing.
    int16_t nADCNum;

    int16_t nTelegraphEnable;
    int16_t nTelegraphInstrument;
    float fTelegraphAdditGain;
    float fTelegraphFilter;
    float fTelegraphMembraneCap;
    int16_t nTelegraphMode;
    float fTelegraphAccessResistance;

    int16_t nADCPtoLChannelMap;
    int16_t nADCSamplingSeq;

    float fADCProgrammableGain;
    float fADCDisplayAmplification;
    float fADCDisplayOffset;
    float fInstrumentScaleFactor;
    float fInstrumentOffset;
    float fSignalGain;
    float fSignalOffset;
    float fSignalLowpassFilter;
    float fSignalHighpassFilter;

    int8_t nLowpassFilterType;
    int8_t nHighpassFilterType;
    float fPostProcessLowpassFilter;
    int8_t nPostProcessLowpassFilterType;
    t_BOOL bEnabledDuringPN;

    int16_t nStatsChannelPolarity;

    int32_t lADCChannelNameIndex;
    int32_t lADCUnitsIndex;

    int8_t sUnused[46];          // size = 128 bytes
};

struct abf2_dacinfo
{
    // The DAC this struct is describing.
    int16_t nDACNum;

    int16_t nTelegraphDACScaleFactorEnable;
    float fInstrumentHoldingLevel;

    float fDACScaleFactor;
    float fDACHoldingLevel;
    float fDACCalibrationFactor;
    float fDACCalibrationOffset;

    int32_t lDACChannelNameIndex;
    int32_t lDACChannelUnitsIndex;

    int32_t lDACFilePtr;
    int32_t lDACFileNumEpisodes;

    int16_t nWaveformEnable;
    int16_t nWaveformSource;
    int16_t nInterEpisodeLevel;

    float fDACFileScale;
    float fDACFileOffset;
    int32_t lDACFileEpisodeNum;
    int16_t nDACFileADCNum;

    int16_t nConditEnable;
    int32_t lConditNumPulses;
    float fBaselineDuration;
    float fBaselineLevel;
    float fStepDuration;
    float fStepLevel;
    float fPostTrainPeriod;
    float fPostTrainLevel;
    int16_t nMembTestEnable;

    int16_t nLeakSubtractType;
    int16_t nPNPolarity;
    float fPNHoldingLevel;
    int16_t nPNNumADCChannels;
    int16_t nPNPosition;
    int16_t nPNNumPulses;
    float fPNSettlingTime;
    float fPNInterpulse;

    int16_t nLTPUsageOfDAC;
    int16_t nLTPPresynapticPulses;

    int32_t lDACFilePathIndex;

    float fMembTestPreSettlingTimeMS;
    float fMembTestPostSettlingTimeMS;

    int16_t nLeakSubtractADCIndex;

    int8_t sUnused[124];     // size = 256 bytes
};

struct abf2_epochinfoperdac
{
    // The Epoch / DAC this struct is describing.
    int16_t nEpochNum;
    int16_t nDACNum;

    // One full set of epochs (ABF_EPOCHCOUNT) for each DAC channel ...
    int16_t nEpochType;
    float fEpochInitLevel;
    float fEpochLevelInc;
    int32_t lEpochInitDuration;
    int32_t lEpochDurationInc;
    int32_t lEpochPulsePeriod;
    int32_t lEpochPulseWidth;

    int8_t sUnused[18];      // size = 48 bytes
};

struct abf2_epochinfo
{
    // The Epoch this struct is describing.
    int16_t nEpochNum;

    // Describes one epoch
    int16_t nDigitalValue;
    int16_t nDigitalTrainValue;
    int16_t nAlternateDigitalValue;
    int16_t nAlternateDigitalTrainValue;
    t_BOOL bEpochCompression;   // Compress the data from this epoch using uFileCompressionRatio

    int8_t sUnused[21];      // size = 32 bytes
} ABF_EpochInfo;

struct abf2_statsregioninfo
{
    // The stats region this struct is describing.
    int16_t nRegionNum;
    int16_t nADCNum;

    int16_t nStatsActiveChannels;
    int16_t nStatsSearchRegionFlags;
    int16_t nStatsSelectedRegion;
    int16_t nStatsSmoothing;
    int16_t nStatsSmoothingEnable;
    int16_t nStatsBaseline;
    int32_t lStatsBaselineStart;
    int32_t lStatsBaselineEnd;

    // Describes one stats region
    int32_t lStatsMeasurements;
    int32_t lStatsStart;
    int32_t lStatsEnd;
    int16_t nRiseBottomPercentile;
    int16_t nRiseTopPercentile;
    int16_t nDecayBottomPercentile;
    int16_t nDecayTopPercentile;
    int16_t nStatsSearchMode;
    int16_t nStatsSearchDAC;
    int16_t nStatsBaselineDAC;

    int8_t sUnused[78];   // size = 128 bytes
};

struct abf2_userlistinfo
{
    // The user list this struct is describing.
    int16_t nListNum;

    // Describes one user list
    int16_t nULEnable;
    int16_t nULParamToVary;
    int16_t nULRepeat;
    int32_t lULParamValueListIndex;

    int8_t sUnused[52];   // size = 64 bytes
};

#endif
