#include <stdint.h>
#include "abf2_print.h"

int abf2_check_section_data_exists(const struct abf2_section *sec);
int64_t abf2_select_section_entry(const struct abf2_section *sec, char *sec_string);
int abf2_seek_to_section_entry(FILE *f, const struct abf2_section *sec, char *sec_string);
int abf2_display_protocolinfo(FILE* f, const struct abf2_section *sec, bool to_swap);
int abf2_display_adcinfo(FILE* f, const struct abf2_section *sec, bool to_swap);
int abf2_display_dacinfo(FILE* f, const struct abf2_section *sec, bool to_swap);
int abf2_display_epochinfoperdac(FILE* f, const struct abf2_section *sec, bool to_swap);
int abf2_display_epochinfo(FILE* f, const struct abf2_section *sec, bool to_swap);
int abf2_display_statsregioninfo(FILE* f, const struct abf2_section *sec, bool to_swap);
int abf2_display_userlistinfo(FILE* f, const struct abf2_section *sec, bool to_swap);
int abf2_display_mathinfo(FILE* f, const struct abf2_section *sec, bool to_swap);

enum ABF2SELECT_OPTIONS {
    ABF2SELECT_FileInfo = 1,
    ABF2SELECT_ProtocolInfo = 2,
    ABF2SELECT_ADCInfo = 3,
    ABF2SELECT_DACInfo = 4,
    ABF2SELECT_EpochInfoPerDAC = 5,
    ABF2SELECT_EpochInfo = 6,
    ABF2SELECT_StatsRegionInfo = 7,
    ABF2SELECT_UserListInfo = 8,
    ABF2SELECT_MathInfo = 9,
    ABF2SELECT_Tags = 10,
    ABF2SELECT_Scopes = 11,
    ABF2SELECT_Deltas = 12,
    ABF2SELECT_VoiceTags = 13,
    ABF2SELECT_Synchs = 14,
    ABF2SELECT_Annotations = 15,
    ABF2SELECT_Stats = 16
};

char *ABF2_SELECTMENU = \
    "MAIN MENU\n\n"
    " ABF2 structs\n"
    " ----------------------\n"
    " [1] FileInfo\n"
    " [2] ProtocolInfo\n"
    " [3] ADCInfo\n"
    " [4] DACInfo\n"
    " [5] EpochInfoPerDAC\n"
    " [6] EpochInfo\n"
    " [7] StatsRegionInfo\n"
    " [8] UserListInfo\n"
    " [9] MathInfo\n"
    "\n ABF structs\n"
    " ----------------------\n"
    " [10] Tags\n"
    " [11] Scopes\n"
    " [12] Deltas\n"
    " [13] VoiceTags\n"
    " [14] Synchs\n"
    " [15] Annotations\n"
    " [16] Stats\n\n"
    "Select a section to show [1-16] (or 0 to exit): ";

char buf[1024];                 /* TODO: lazy alert -- global buffer */


int abf2_check_section_data_exists(const struct abf2_section *sec)
{
    if (sec->uBlockIndex == 0 || sec->llNumEntries == 0) {
        printf("No info for this section in this file\n\n");
        return 0;
    }
    return 1;
}

int64_t abf2_select_section_entry(const struct abf2_section *sec, char *sec_string)
{
    int64_t entry_count = sec->llNumEntries;
    if (entry_count > 1) {
        int64_t entry_select = -1;
        while (entry_select == -1) {
            printf("\nSelect an entry to show [1-%ld] (or 0 to exit): ",
                   entry_count, sec_string, entry_count);
            scanf("%ld", &entry_select); /* TODO: stop using scanf */
            if (entry_select > entry_count) {/* error check */
                entry_select = -1;
            }
            printf("\n");
        }
        return entry_select;
    } else {
        return 1;               /* only one entry, so select it! */
    }
}

int abf2_seek_to_section_entry(FILE *f, const struct abf2_section *sec, char *sec_string)
{
    int64_t eselect = abf2_select_section_entry(sec, sec_string);
    if (eselect == 0) return 0;

    int block_offset = abf2_section_block_offset(sec);
    int entry_offset = sec->uBytes * (eselect-1);
    fseek(f, block_offset + entry_offset, SEEK_SET);
    return eselect;
}

int abf2_display_protocolinfo(FILE* f, const struct abf2_section *sec, bool to_swap)
{
    if (0 == abf2_check_section_data_exists(sec)) {
        return 0;
    }

    int64_t entry_select = -1;
    while (entry_select != 0) {
        entry_select = abf2_seek_to_section_entry(f, sec, "ProtocolInfo");
        if (entry_select == 0)
            return 0; /* exit on selecting 0 */

        struct abf2_protocolinfo *pinfo;
        pinfo = (struct abf2_protocolinfo *) malloc(sizeof(struct abf2_protocolinfo));
        if (0 == fread(buf, (size_t)sec->uBytes, 1, f)) {
            printf("Read Protocol info error\n");
            return -1;
        }
        abf2_read_protocolinfo(buf, pinfo, to_swap);
        printf("\n");
        abf2_print_protocolinfo(pinfo, 0);
        printf("\n");
        if (sec->llNumEntries == 1)
            entry_select = 0;
        else {
            entry_select = -1;
            printf("---------------------------------------------\n");
        }
    }
    return 0;
}

int abf2_display_adcinfo(FILE* f, const struct abf2_section *sec, bool to_swap)
{
    if (0 == abf2_check_section_data_exists(sec)) {
        return 0;
    }

    int64_t entry_select = -1;
    while (entry_select != 0) {
        entry_select = abf2_seek_to_section_entry(f, sec, "ADCInfo");
        if (entry_select == 0)
            return 0; /* exit on selecting 0 */

        struct abf2_adcinfo *ainfo;
        ainfo = (struct abf2_adcinfo *) malloc(sizeof(struct abf2_adcinfo));
        if (0 == fread(buf, ABF2_ADCINFOSIZE, 1, f)) {
            printf("Read ADC info error\n");
            return -1;
        }
        abf2_read_adcinfo(buf, ainfo, to_swap);
        printf("\n");
        abf2_print_adcinfo(ainfo, 0);
        printf("\n");
        if (sec->llNumEntries == 1)
            entry_select = 0;
        else {
            entry_select = -1;
            printf("---------------------------------------------\n");
        }
    }
    return 0;
}

int abf2_display_dacinfo(FILE* f, const struct abf2_section *sec, bool to_swap)
{
    if (0 == abf2_check_section_data_exists(sec)) {
        return 0;
    }

    int64_t entry_select = -1;
    while (entry_select != 0) {
        entry_select = abf2_seek_to_section_entry(f, sec, "DACInfo");
        if (entry_select == 0)
            return 0; /* exit on selecting 0 */

        struct abf2_dacinfo *ainfo;
        ainfo = (struct abf2_dacinfo *) malloc(sizeof(struct abf2_dacinfo));
        if (0 == fread(buf, ABF2_DACINFOSIZE, 1, f)) {
            printf("Read DAC info error\n");
            return -1;
        }
        abf2_read_dacinfo(buf, ainfo, to_swap);
        printf("\n");
        abf2_print_dacinfo(ainfo, 0);
        printf("\n");
        if (sec->llNumEntries == 1)
            entry_select = 0;
        else {
            entry_select = -1;
            printf("---------------------------------------------\n");
        }
    }
    return 0;
}

int abf2_display_epochinfoperdac(FILE* f, const struct abf2_section *sec, bool to_swap)
{
    if (0 == abf2_check_section_data_exists(sec)) {
        return 0;
    }

    int64_t entry_select = -1;
    while (entry_select != 0) {
        entry_select = abf2_seek_to_section_entry(f, sec, "EpochInfoPerDAC");
        if (entry_select == 0)
            return 0; /* exit on selecting 0 */

        struct abf2_epochinfoperdac *ainfo;
        ainfo = (struct abf2_epochinfoperdac *) malloc(sizeof(struct abf2_epochinfoperdac));
        if (0 == fread(buf, ABF2_EPOCHINFOPERDACSIZE, 1, f)) {
            printf("Read DAC info error\n");
            return -1;
        }
        abf2_read_epochinfoperdac(buf, ainfo, to_swap);
        printf("\n");
        abf2_print_epochinfoperdac(ainfo, 0);
        printf("\n");
        if (sec->llNumEntries == 1)
            entry_select = 0;
        else {
            entry_select = -1;
            printf("---------------------------------------------\n");
        }
    }
    return 0;
}

int abf2_display_epochinfo(FILE* f, const struct abf2_section *sec, bool to_swap)
{
    if (0 == abf2_check_section_data_exists(sec)) {
        return 0;
    }

    int64_t entry_select = -1;
    while (entry_select != 0) {
        entry_select = abf2_seek_to_section_entry(f, sec, "EpochInfo");
        if (entry_select == 0)
            return 0; /* exit on selecting 0 */

        struct abf2_epochinfo *ainfo;
        ainfo = (struct abf2_epochinfo *) malloc(sizeof(struct abf2_epochinfo));
        if (0 == fread(buf, ABF2_EPOCHINFOSIZE, 1, f)) {
            printf("Read DAC info error\n");
            return -1;
        }
        abf2_read_epochinfo(buf, ainfo, to_swap);
        printf("\n");
        abf2_print_epochinfo(ainfo, 0);
        printf("\n");
        if (sec->llNumEntries == 1)
            entry_select = 0;
        else {
            entry_select = -1;
            printf("---------------------------------------------\n");
        }
    }
    return 0;
}

int abf2_display_statsregioninfo(FILE* f, const struct abf2_section *sec, bool to_swap)
{
    if (0 == abf2_check_section_data_exists(sec)) {
        return 0;
    }

    int64_t entry_select = -1;
    while (entry_select != 0) {
        entry_select = abf2_seek_to_section_entry(f, sec, "StatsRegionInfo");
        if (entry_select == 0)
            return 0; /* exit on selecting 0 */

        struct abf2_statsregioninfo *ainfo;
        ainfo = (struct abf2_statsregioninfo *) malloc(sizeof(struct abf2_statsregioninfo));
        if (0 == fread(buf, ABF2_STATSREGIONINFOSIZE, 1, f)) {
            printf("Read StatsRegion info error\n");
            return -1;
        }
        abf2_read_statsregioninfo(buf, ainfo, to_swap);
        printf("\n");
        abf2_print_statsregioninfo(ainfo, 0);
        printf("\n");
        if (sec->llNumEntries == 1)
            entry_select = 0;
        else {
            entry_select = -1;
            printf("---------------------------------------------\n");
        }
    }
    return 0;
}

int abf2_display_userlistinfo(FILE* f, const struct abf2_section *sec, bool to_swap)
{
    if (0 == abf2_check_section_data_exists(sec)) {
        return 0;
    }

    int64_t entry_select = -1;
    while (entry_select != 0) {
        entry_select = abf2_seek_to_section_entry(f, sec, "UserListInfo");
        if (entry_select == 0)
            return 0; /* exit on selecting 0 */

        struct abf2_userlistinfo *ainfo;
        ainfo = (struct abf2_userlistinfo *) malloc(sizeof(struct abf2_userlistinfo));
        if (0 == fread(buf, ABF2_USERLISTINFOSIZE, 1, f)) {
            printf("Read UserList info error\n");
            return -1;
        }
        abf2_read_userlistinfo(buf, ainfo, to_swap);
        printf("\n");
        abf2_print_userlistinfo(ainfo, 0);
        printf("\n");
        if (sec->llNumEntries == 1)
            entry_select = 0;
        else {
            entry_select = -1;
            printf("---------------------------------------------\n");
        }
    }
    return 0;
}

int abf2_display_mathinfo(FILE* f, const struct abf2_section *sec, bool to_swap)
{
    if (0 == abf2_check_section_data_exists(sec)) {
        return 0;
    }

    int64_t entry_select = -1;
    while (entry_select != 0) {
        entry_select = abf2_seek_to_section_entry(f, sec, "MathInfo");
        if (entry_select == 0)
            return 0; /* exit on selecting 0 */

        struct abf2_mathinfo *ainfo;
        ainfo = (struct abf2_mathinfo *) malloc(sizeof(struct abf2_mathinfo));
        if (0 == fread(buf, ABF2_MATHINFOSIZE, 1, f)) {
            printf("Read Math info error\n");
            return -1;
        }
        abf2_read_mathinfo(buf, ainfo, to_swap);
        printf("\n");
        abf2_print_mathinfo(ainfo, 0);
        printf("\n");
        if (sec->llNumEntries == 1)
            entry_select = 0;
        else {
            entry_select = -1;
            printf("---------------------------------------------\n");
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    FILE *f;

    if (argc < 2) {
        printf("Usage: %s <file>\n", argv[0]);
        exit(1);
    }

    char *fname = argv[1];
    if (fname == NULL) fname = "";

    printf("\nABF2 (Axon Binary Format v2) File Reader\n");
    printf("=============================================\n\n");

    printf("Active File: %s\n\n", fname);

    f = fopen(fname, "r");
    if (f == NULL) {
        fprintf(stderr, "Can't open the given file. Check file path.\n");
        exit(1);
    }

    struct abf2_fileinfo *finfo;
    finfo = (struct abf2_fileinfo *) malloc(sizeof(struct abf2_fileinfo));
    if (finfo == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }

    if (0 == fread(buf, ABF2_FILEINFOSIZE, 1, f)) {
        printf("Read error. Check that the file is an ABF2 file?\n");
        exit(1);
    }

    if (0 == abf2_can_open(buf)) {
        printf("Can't read non-ABF2 files.\n");
        exit(1);
    }

    int to_swap = abf2_needs_swap(buf);
    abf2_read_fileinfo(buf, finfo, to_swap);

    int struct_select = -1;
    struct abf2_section *sec;
    while (struct_select != 0) {
        printf("=============================================\n\n");
        printf("%s", ABF2_SELECTMENU);
        /* TODO: laziness alert -- using scanf */
        scanf("%d", &struct_select);

        if (struct_select == 0) exit(0);
        printf("\n=============================================\n\n");
        switch(struct_select) {
        case ABF2SELECT_FileInfo:
            printf("FileInfo\n");
            abf2_print_fileinfo(finfo, 2);
            break;
        case ABF2SELECT_ProtocolInfo:
            sec = &(finfo->ProtocolSection);
            printf("ProtocolSection\n");
            abf2_print_section(sec, 2);
            abf2_display_protocolinfo(f, sec, to_swap);
            break;
        case ABF2SELECT_ADCInfo:
            sec = &(finfo->ADCSection);
            printf("ADCSection\n");
            abf2_print_section(sec, 2);
            abf2_display_adcinfo(f, sec, to_swap);
            break;
        case ABF2SELECT_DACInfo:
            sec = &(finfo->DACSection);
            printf("DACSection\n");
            abf2_print_section(sec, 2);
            abf2_display_dacinfo(f, sec, to_swap);
            break;
        case ABF2SELECT_EpochInfoPerDAC:
            sec = &(finfo->EpochPerDACSection);
            printf("EpochPerDACSection\n");
            abf2_print_section(sec, 2);
            abf2_display_epochinfoperdac(f, sec, to_swap);
            break;
        case ABF2SELECT_EpochInfo:
            sec = &(finfo->EpochSection);
            printf("EpochSection\n");
            abf2_print_section(sec, 2);
            abf2_display_epochinfo(f, sec, to_swap);
            break;
        case ABF2SELECT_StatsRegionInfo:
            sec = &(finfo->StatsRegionSection);
            printf("StatsRegionSection\n");
            abf2_print_section(sec, 2);
            abf2_display_statsregioninfo(f, sec, to_swap);
            break;
        case ABF2SELECT_UserListInfo:
            sec = &(finfo->UserListSection);
            printf("UserListSection\n");
            abf2_print_section(sec, 2);
            abf2_display_userlistinfo(f, sec, to_swap);
            break;
        case ABF2SELECT_MathInfo:
            sec = &(finfo->MathSection);
            printf("MathSection\n");
            abf2_print_section(sec, 2);
            abf2_display_mathinfo(f, sec, to_swap);
            break;
        /* case ABF2SELECT_Tags: */
        /*     abf2_display_tags(f, finfo, to_swap); */
        /*     break; */
        /* case ABF2SELECT_Scopes: */
        /*     abf2_display_scopes(f, finfo, to_swap); */
        /*     break; */
        /* case ABF2SELECT_Deltas: */
        /*     abf2_display_deltas(f, finfo, to_swap); */
        /*     break; */
        /* case ABF2SELECT_VoiceTags: */
        /*     abf2_display_voicetags(f, finfo, to_swap); */
        /*     break; */
        /* case ABF2SELECT_Synchs: */
        /*     abf2_display_synchs(f, finfo, to_swap); */
        /*     break; */
        /* case ABF2SELECT_Annotations: */
        /*     abf2_display_annotations(f, finfo, to_swap); */
        /*     break; */
        /* case ABF2SELECT_Stats: */
        /*     abf2_display_stats(f, finfo, to_swap); */
        /*     break; */
        default:
            printf("Unknown option. Try again, please.\n\n");
            break;
        }
    }

    fclose(f);
    return 0;
}
