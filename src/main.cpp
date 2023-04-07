#include "tx/tx.h"
#include "rx/rx.h"

int main(int argc, char* argv[]) {
    pid_t pid = fork();

    // start Receiver
    if (pid == 0) {
        Receiver rec("54000", 10);
        rec.run();
        exit(0);
    }

    // start Transmitter
    else {
        Transmitter tr;
        tr.run();
        kill(pid, SIGKILL);
    }

    return 0;
};

