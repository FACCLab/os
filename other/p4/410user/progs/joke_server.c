/* @author Funny you should ask... */

#include <stdio.h>
#include <stdbool.h>
#include <syscall.h>
#include <rand.h>
#include <ipc_server.h>
#include <thread.h>

#define BUF_LEN 1024

struct {
    uint8_t cmd;
    char buf[BUF_LEN];
} req;

// Shamelessly stolen from "www.gotlines.com" and 
// "http://www.danoah.com/2013/01/best-short-jokes-ever.html",
// and "http://www.laughfactory.com/jokes/word-play-jokes" 
// and "http://www.tickld.com/x/21-best-one-liner-jokes-15-is-just-evil" 
// (don't worry didn't include #15)
char* jokes[] = {
    "How do you make holy water? Boil the hell out of it!",
    "What do you call a bear with no teeth? A gummy bear!",
    "I wondered why the frisbee was getting bigger, and then it hit me!",
    "Never argue with a fool, they will lower you to their level and then beat"
        " you with experience!",
    "What did the fish say when he swam into the wall? DAM!",
    "The early bird might get the worm, but the second mouse gets the cheese!",
    "What did the ocean say to the beach? Nothing, it just waved!",
    "What do you call a sheep with no legs? A cloud.",
    "Why can't you hear a pterodactyl in the bathroom? Because it has a silent"
        " pee!",
    "What did the Zen Buddhist say to the hotdog vendor? Make me one with"
        " everything.",
    "What is Beethoven's favorite fruit? Ba-na-na-naaaa!",
    "What do you call a deer with no eyes? No eye deer!",
    "The past, present, and future walk into a bar. It was tense.",
    "What did the grape say when he was pinched? Nothing, he gave a little" 
        " wine.",
    "What do you get when you throw a piano down a mine shaft? A flat miner!",
    "What do you get when you put a candle in a suit of armor? A knight"
        " light!",
    "Have you heard about corduroy pillows? They're making headlines!", // this is a good one
    "Two drums and a cymbal fall off a cliff. Ba-dum tish.",
    "A snake, a drum, and a sheep fall off a cliff. Bah-dum tss.",
    "There were two peanuts walking down a dark alley, one was assaulted.",
    "What do you call a sleekpwalking nun? A roamin' Catholic.",
    "What did the 0 say to the 8? Nice belt!",
    "Why did the orange stop? Because it ran outta juice!",
    "Why did the stormtrooper buy an iPhone? He couldn't find the Droid he was"
        "looking for!", //this one too
    "Bacon and eggs walk into a bar and order a beer, the bartender says"
        " \"Sorry, we don't serve breakfast.\"",
    "What happens to a frog's car when it breaks down? It gets toad away.",
    "How do you count cows? With a cowculator!",
    "How do astronomers organize a party? They planet!",
    "Did you hear about the kidnapping at school? It's okay, he woke up.",
    "A man got hit in the head with a can of Coke. It's okay though, it's a"
        " soft drink.",
    "I hate Russian dolls, they're so full of themselves.",
    "My wife told me to stop impersonating a flamingo. I had to put my foot"
        " down.",
    "I haven't slept for three days, because that would be too long.",
    "My grandfather has the heart of a lion and a lifetime ban from the local" 
        " zoo.",
    "I recently decided to sell my vacuum cleaner, all it was doing was"
        " gathering dust.",
    "Whiteboards are remarkable.",
    "I saw a sign that said \"Watch for Children\". I thought that sounded" 
        " like a fair trade."
};

#define NJOKES (sizeof(jokes)/sizeof(jokes[0]))

int main() {

    sgenrand(gettid() * get_ticks());

    int pid;
    if ((pid = fork()) != 0) {
        if (pid < 0) {
            printf("joke server could not be started\n");
            return -1;
        } else {
            printf("joke server started on pid %d\n", pid);
            return 0;
        }
    }    

    // So that we can use malloc
    thr_init(4096);

    ipc_state_t* server_st;
    if (ipc_server_init(&server_st, UDR_JOKE_SERVER) < 0) {
        printf("could not register for joke server, exiting...\n");
        return -1;
    }

    while (true) {
        // Receive a request for a joke.
        driv_id_t sender;
        if (ipc_server_recv(server_st, &sender, &req, sizeof(req), true) < 0) {
            printf("could not receive request, exiting...\n");
            ipc_server_cancel(server_st);
            return -1;
        }

        ipc_server_send_str(server_st, sender, jokes[genrand() % NJOKES]);
    }
}
