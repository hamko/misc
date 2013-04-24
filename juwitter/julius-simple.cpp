/* include top Julius library header */
#include <iostream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <time.h>
#include <map>

#include "twitterPostTweet.hpp"
#include "twitterGetTimeline.hpp"

using namespace std;

typedef void (*proc) (void);
map<vector<string>, proc> mapping;

#include <julius/juliuslib.h>

#define printfunc() 
//#define printfunc() (cout << "#" << __func__ << endl)

/** 
 * Callback to be called when start waiting speech input. 
 * 
 */
    static void
status_recready(Recog *recog, void *dummy)
{
    printfunc();
    if (recog->jconf->input.speech_input == SP_MIC || recog->jconf->input.speech_input == SP_NETAUDIO) {
        fprintf(stderr, "<<< please speak >>>");
    }
}

/** 
 * Callback to be called when speech input is triggered.
 * 
 */
static void status_recstart(Recog *recog, void *dummy)
{
    printfunc();
    if (recog->jconf->input.speech_input == SP_MIC || recog->jconf->input.speech_input == SP_NETAUDIO) {
        fprintf(stderr, "\r                    \r");
    }
}

/** 
 * Sub function to output phoneme sequence.
 * 
 */
static void put_hypo_phoneme(WORD_ID *seq, int n, WORD_INFO *winfo)
{
    printfunc();
    int i,j;
    WORD_ID w;
    static char buf[MAX_HMMNAME_LEN];

    if (seq != NULL) {
        for (i=0;i<n;i++) {
            if (i > 0) printf(" |");
            w = seq[i];
            for (j=0;j<winfo->wlen[w];j++) {
                center_name(winfo->wseq[w][j]->name, buf);
                printf(" %s", buf);
            }
        }
    }
    printf("\n");  
}

void hoge(void)
{
    ostringstream os;
    os << "はむっ #音声認識でツイート " << rand() % 10000;
    send_tweet(os.str());
}

void init_command_func_mapping(void)
{
    vector <string> a;

    a.clear();
    a.push_back("command"); a.push_back("tweet"); a.push_back("testone");
    sort(a.begin(), a.end());
    mapping[a] = hoge;
}

void call_func_with_command(char** command, WORD_ID* index, int num)
{
    vector <string> cm;
    for (int i = 0; i < num; i++)
        cm.push_back(command[index[i]]);
    sort(cm.begin(), cm.end());

    map<vector<string>, proc>::iterator it;
    it = mapping.find(cm);

    if (it == mapping.end()) {
        cout << "No such command!" << endl;
    } else {
        mapping[cm]();
    }
}


/** 
 * Parse Command
 */
static void parse_command(RecogProcess *r)
{
    WORD_ID *seq;
    int seqnum;
    Sentence *s;
    WORD_INFO *winfo = r->lm->winfo;
    for(int n = 0; n < r->result.sentnum; n++) { /* for all sentences */
        s = &(r->result.sent[n]);
        seq = s->word;
        seqnum = s->word_num;

        /*
        printf("sentence%d:", n+1);
        for(int i=0;i<seqnum;i++) 
            printf(" %s", winfo->woutput[seq[i]]);
        printf("cmscore%d:", n+1);
        for (int i=0;i<seqnum; i++)
            printf(" %5.3f", s->confidence[i]);
            */
        bool faf = true;
        for (int i = 0; i < seqnum; i++) {
            if (s->confidence[i] < 0.95) {
                faf = false;
                break;
            }
        }
        if (faf) {
            cout << "Good Confident!!";
            for(int i=0;i<seqnum;i++) 
                cout << winfo->woutput[seq[i]] << " ";
            cout << endl;
            call_func_with_command(winfo->woutput, seq, seqnum);
        } else {
            cout << "Lacking for Confident.";
        }
    }
}

/** 
 * Print output information
 *
 */
static void print_output_info(RecogProcess *r)
{
    HMM_Logical *p;
    SentenceAlign *align;
    WORD_ID *seq;
    int seqnum;
    Sentence *s;
    WORD_INFO *winfo = r->lm->winfo;
    for(int n = 0; n < r->result.sentnum; n++) { /* for all sentences */

        s = &(r->result.sent[n]);
        seq = s->word;
        seqnum = s->word_num;

        /* output word sequence like Julius */
        printf("sentence%d:", n+1);
        for(int i=0;i<seqnum;i++) printf(" %s", winfo->woutput[seq[i]]);
        printf("\n");
        /* LM entry sequence */
        printf("wseq%d:", n+1);
        for(int i=0;i<seqnum;i++) printf(" %s", winfo->wname[seq[i]]);
        printf("\n");
        /* phoneme sequence */
        printf("phseq%d:", n+1);
        put_hypo_phoneme(seq, seqnum, winfo);
        printf("\n");
        /* confidence scores */
        printf("cmscore%d:", n+1);
        for (int i=0;i<seqnum; i++) printf(" %5.3f", s->confidence[i]);
        printf("\n");
        /* AM and LM scores */
        printf("score%d: %f", n+1, s->score);
        if (r->lmtype == LM_PROB) { /* if this process uses N-gram */
            printf(" (AM: %f  LM: %f)", s->score_am, s->score_lm);
        }
        printf("\n");
        if (r->lmtype == LM_DFA) { /* if this process uses DFA grammar */
            /* output which grammar the hypothesis belongs to
               when using multiple grammars */
            if (multigram_get_all_num(r->lm) > 1) {
                printf("grammar%d: %d\n", n+1, s->gram_id);
            }
        }

        /* output alignment result if exist */
        for (align = s->align; align; align = align->next) {
            printf("=== begin forced alignment ===\n");
            switch(align->unittype) {
                case PER_WORD:
                    printf("-- word alignment --\n"); break;
                case PER_PHONEME:
                    printf("-- phoneme alignment --\n"); break;
                case PER_STATE:
                    printf("-- state alignment --\n"); break;
            }
            printf(" id: from  to    n_score    unit\n");
            printf(" ----------------------------------------\n");
            for(int i=0;i<align->num;i++) {
                printf("[%4d %4d]  %f  ", align->begin_frame[i], align->end_frame[i], align->avgscore[i]);
                switch(align->unittype) {
                    case PER_WORD:
                        printf("%s\t[%s]\n", winfo->wname[align->w[i]], winfo->woutput[align->w[i]]);
                        break;
                    case PER_PHONEME:
                        p = align->ph[i];
                        if (p->is_pseudo) {
                            printf("{%s}\n", p->name);
                        } else if (strmatch(p->name, p->body.defined->name)) {
                            printf("%s\n", p->name);
                        } else {
                            printf("%s[%s]\n", p->name, p->body.defined->name);
                        }
                        break;
                    case PER_STATE:
                        p = align->ph[i];
                        if (p->is_pseudo) {
                            printf("{%s}", p->name);
                        } else if (strmatch(p->name, p->body.defined->name)) {
                            printf("%s", p->name);
                        } else {
                            printf("%s[%s]", p->name, p->body.defined->name);
                        }
                        if (r->am->hmminfo->multipath) {
                            if (align->is_iwsp[i]) {
                                printf(" #%d (sp)\n", align->loc[i]);
                            } else {
                                printf(" #%d\n", align->loc[i]);
                            }
                        } else {
                            printf(" #%d\n", align->loc[i]);
                        }
                        break;
                }
            }

            printf("re-computed AM score: %f\n", align->allscore);

            printf("=== end forced alignment ===\n");
        }
    }
}

/** 
 * Callback to output final recognition result.
 * This function will be called just after recognition of an input ends
 * 
 */
    static void
output_result(Recog *recog, void *dummy)
{
    printfunc();
    RecogProcess *r;

    /* all recognition results are stored at each recognition process
       instance */
    for(r=recog->process_list;r;r=r->next) {

        /* skip the process if the process is not alive */
        if (! r->live) continue;

        /* result are in r->result.  See recog.h for details */

        /* check result status */
        if (r->result.status < 0) {      /* no results obtained */
            /* outout message according to the status code */
            switch(r->result.status) {
                case J_RESULT_STATUS_REJECT_POWER:
                    printf("<input rejected by power>\n");
                    break;
                case J_RESULT_STATUS_TERMINATE:
                    printf("<input teminated by request>\n");
                    break;
                case J_RESULT_STATUS_ONLY_SILENCE:
                    printf("<input rejected by decoder (silence input result)>\n");
                    break;
                case J_RESULT_STATUS_REJECT_GMM:
                    printf("<input rejected by GMM>\n");
                    break;
                case J_RESULT_STATUS_REJECT_SHORT:
                    printf("<input rejected by short input>\n");
                    break;
                case J_RESULT_STATUS_FAIL:
                    printf("<search failed>\n");
                    break;
            }
            /* continue to next process instance */
            continue;
        }

        /* output results for all the obtained sentences */
        print_output_info(r);
        parse_command(r);

        /* flush output buffer */
        fflush(stdout);
    }
}

/**
 * Main function
 * 
 */
void* juwitter_post_main(void* args)
{
    cout << "called juwitter_post_main" << endl;
    char* argv[] = {
    "./julius-simple",
    "-gram",
    "tools/command",
    "-h",
    "model/phone_m/hmmdefs_ptm_gid.binhmm",
    "-hlist",
    "model/phone_m/logicalTri",
    "-input",
    "mic",
    };
    int argc = 9;

    srand((unsigned int)time(NULL));
    // initialize command-func mapping
    init_command_func_mapping();

    /**
     * configuration parameter holder
     * 
     */
    Jconf *jconf;

    /**
     * Recognition instance
     * 
     */
    Recog *recog;

    /**
     * speech file name for MFCC file input
     * 
     */
    static char speechfilename[MAXPATHLEN];

    int ret;

    /* by default, all messages will be output to standard out */
    /* to disable output, uncomment below */
    //jlog_set_output(NULL);

    /* output log to a file */
    //FILE *fp; fp = fopen("log.txt", "w"); jlog_set_output(fp);

    /* if no argument, output usage and exit */
    if (argc == 1) {
        fprintf(stderr, "Julius rev.%s - based on ", JULIUS_VERSION);
        j_put_version(stderr);
        fprintf(stderr, "Try '-setting' for built-in engine configuration.\n");
        fprintf(stderr, "Try '-help' for run time options.\n");
        return NULL;
    }

    /************/
    /* Start up */
    /************/
    /* 1. load configurations from command arguments */
    jconf = j_config_load_args_new(argc, argv);
    /* else, you can load configurations from a jconf file */
    //jconf = j_config_load_file_new(jconf_filename);
    if (jconf == NULL) {        /* error */
        fprintf(stderr, "Try `-help' for more information.\n");
        return NULL;
    }

    /* 2. create recognition instance according to the jconf */
    /* it loads models, setup final parameters, build lexicon
       and set up work area for recognition */
    recog = j_create_instance_from_jconf(jconf);
    if (recog == NULL) {
        fprintf(stderr, "Error in startup\n");
        return NULL;
    }

    /*********************/
    /* Register callback */
    /*********************/
    /* register result callback functions */
    callback_add(recog, CALLBACK_EVENT_SPEECH_READY, status_recready, NULL);
    callback_add(recog, CALLBACK_EVENT_SPEECH_START, status_recstart, NULL);
    callback_add(recog, CALLBACK_RESULT, output_result, NULL);

    /**************************/
    /* Initialize audio input */
    /**************************/
    /* initialize audio input device */
    /* ad-in thread starts at this time for microphone */
    if (j_adin_init(recog) == FALSE) {    /* error */
        return NULL;
    }

    /* output system information to log */
    j_recog_info(recog);

    /***********************************/
    /* Open input stream and recognize */
    /***********************************/

    if (jconf->input.speech_input == SP_MFCFILE) {
        /* MFCC file input */

        while (get_line_from_stdin(speechfilename, MAXPATHLEN, (char*)"enter MFCC filename->") != NULL) {
            if (verbose_flag) printf("\ninput MFCC file: %s\n", speechfilename);
            /* open the input file */
            ret = j_open_stream(recog, speechfilename);
            switch(ret) {
                case 0:            /* succeeded */
                    break;
                case -1:              /* error */
                    /* go on to the next input */
                    continue;
                case -2:            /* end of recognition */
                    return NULL;
            }
            /* recognition loop */
            ret = j_recognize_stream(recog);
            if (ret == -1) return NULL;    /* error */
            /* reach here when an input ends */
        }

    } else {
        /* raw speech input (microphone etc.) */

        switch(j_open_stream(recog, NULL)) {
            case 0:            /* succeeded */
                break;
            case -1:              /* error */
                fprintf(stderr, "error in input stream\n");
                return NULL;
            case -2:            /* end of recognition process */
                fprintf(stderr, "failed to begin input stream\n");
                return NULL;
        }

        /**********************/
        /* Recognization Loop */
        /**********************/
        /* enter main loop to recognize the input stream */
        /* finish after whole input has been processed and input reaches end */
        ret = j_recognize_stream(recog);
        if (ret == -1) return NULL;    /* error */

        /*******/
        /* End */
        /*******/
    }

    /* calling j_close_stream(recog) at any time will terminate
       recognition and exit j_recognize_stream() */
    j_close_stream(recog);

    j_recog_free(recog);

    /* exit program */
    return NULL;
}
