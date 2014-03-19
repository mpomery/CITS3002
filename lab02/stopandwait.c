#include <cnet.h>
#include <stdlib.h>
#include <string.h>

/*  This file provides a "template" of event-handling functions that will
    help you complete the stopandwait protocol from Lecture-3.
 */

typedef enum { DL_DATA, DL_ACK } FRAMEKIND;

typedef struct { char data[MAX_MESSAGE_SIZE]; } MSG;

typedef struct {
    FRAMEKIND kind;
    size_t    len;
    int       checksum;
    int       seq;
    MSG       msg;
} FRAME;

#define FRAME_HEADER_SIZE (sizeof(FRAME) - sizeof(MSG))
#define FRAME_SIZE(f)     (FRAME_HEADER_SIZE + f.len))

static MSG lastmsg;
static size_t lastlength = 0;
static CnetTimerID lasttimer = NULLTIMER;

static int ackexpected = 0;
static int nextframetosend = 0;
static int frameexpected = 0;

/*  This function will be invoked each time the application layer has a
    message to deliver. Once we read the message from the application layer
    with CNET_read_application(), the application layer will be able to supply
    us with another message. The rate of new messages is controlled by
    the 'messagerate' node attribute.
 */

static EVENT_HANDLER(application_ready)
{
    CnetAddr	destaddr;

    lastlength = sizeof(MSG);
    CHECK(CNET_read_application(&destaddr, buffer, &length));
    CNET_disable_applications(ALLNODES);

    printf("down from application, seq=%d\n", nextframetosend);

    transmit_frame(&lastmsg, DL_DATA, lastlength, nextframetosend);

    nextfametosend = 1 - nextframetosend;
}

static void transmit_frame(MSG *msg, FRAMEKIND kind, size_t msglen, int seqno)
{
    FRAME f;
    f.kind = kind;
    f.seq = seqno;
    f.checksum = 0;
    f.len = msglen;

    switch(kind) {
        case DL_ACK:
            printf("ACK transmitted, seq=%d\n", seqno)
            break;
        case DL_DATA:
            CnetTime timeout;
            memcpy(&f.msg, msg, msglen);
            printf(" DL_DATA transmitted, seq=%d\n", seqno);

            timeout = (FRAME_SIZE(f)*8000000 / linkinfo[1].bandwidth) +
                linkinfo[1].propegationdelay;
            lasttimer = CNET_start_timer(EV_TIMER1, timeout, 0);

            break;
    }
    msglen = FRAM_SIZE(f);
    f.checksum = CNET_ccitt((unsigned char *)&f, (int) msglen);

    CHECK(CNET_write_physical(1, &f, &msglen));
}

static EVENT_HANDLER(physical_ready)
{
    FRAME f;
    size_t len;
    int link, checksum;

    len = sizeof(FRAME);
    CHECK(CNET_read_physical(&link, &f, &len));

    checksum = f.checksum;
    f.checksum = 0;
    if (CNET_ccitt((unsigned char *) &f, (int) len) != checksum) {
        printf("\t\t\t\tBAD checksum - frame ignored\n");
        return; // Ignore Frame
    }

    switch(f.kind) {
      case DL_ACK : {
        if(f.seq == ackexpected) {
            printf("\t\t\t\tACK received, seq=%d\n", f.seq);
            CNET_stop_timer(lasttimer);
            ackexpected = 1-ackexpected;
            CNET_enable_application(ALLNODES);
        }
        break;
      }
      case DL_DATA : {
        printf("\t\t\t\tDATA received, seq=%d, ", f.seq);
        if(f.seq == frameexpected) {
            printf("up to application\n");
            len = f.len;
            CHECK(CNET_write_application(&f.msg, &len));
            frameexpected = 1-frameexpected;
        }
        else
            printf("ignored\n");

        transmit_frame((MSG *)NULL, DL_ACK, 0, f.seq);
        break;
      }
    }
}

static EVENT_HANDLER(button_pressed)
{
    printf("\n Node number     : %d\n", nodeinfo.nodenumber);
    printf(" Node address    : %d\n", nodeinfo.address);
    printf(" Node name       : %s\n", nodeinfo.nodename);
    printf(" Number of links : %d\n\n", nodeinfo.nlinks);
}

static EVENT_HANDLER(showstate)
{
    printf("\tackexpected\t= %d\n
        \tnextframetosend\t= %d\n
        \tframeexpected\t= %d\n",
        ackexpected, nextframetosend, frameexpected);
}

// Happens whenever the node comes online
EVENT_HANDLER(reboot_node)
{
/*  Indicate that we are interested in hearing about the Application Layer
    having messages for delivery and indicate which function to invoke for them.
 */
    if (nodeinfo.nodenumber > 1) {
        fprintf(stderr, "This is a 2 node network!\n");
        exit(1);
    }

    // CNET_set_handler(event, function to call, unknown); 
    CHECK(CNET_set_handler(EV_APPLICATIONREADY, application_ready, 0));
    CHECK(CNET_set_handler(EV_PHYSICALREADY, physical_ready, 0));
    CHECK(CNET_set_handler(EV_TIMER1, timeouts, 0));
    CHECK(CNET_set_handler(EV_DEBUG1, button_pressed, 0));

    CHECK(CNET_set_debug_string(EV_DEBUG1, "Node Info"));

    if (nodeinfo.nodenumber == 1)
        CNET_enable_application(ALLNODES);
}
