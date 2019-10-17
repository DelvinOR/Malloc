We designed a struct called metadata or md for short that we used to easily store our metadata in our array. Metadata was defined as such:
int size;
char inUse;
char key1;
char key2;
struct metadata* next;

The total size of our metadata was 16 bytes. The extra byte is probably due to padding an extra byte after our 3 chars.

It is of course possible to optimize out metadata much further, such as reducing size to a short, or getting rid of it altogether,  or getting rid of a key. Also, we use a whole byte for checking if something is in use, when we only really need 1 bit. This means we could probably union this bit with some other metadata field, per say a short, and since size would never be negative, have that extra bit for negative be used for our in use flag.

If we really wanted to go all out, we could probably get rid of most of the metadata, including perhaps most of our metadata pointer. We could instead simply store an offset, and add that to the address of &myblock (our array) to save a lot on space.

key1 and key2 were just used as checks to verify that calls to free made sense, that a block was in use and existed.
We defined the check for key1 == 0xFF and key2 == 0x55.
 
inUse was a char, either 'y' or 'n' that would signal whether a block was currently allocated to someone.
Our metadata* was a pointer to the next block of metadata in our array, with the allocated space for our current metadata inbetween.
 
To check if our my_malloc funciton had been called previously, and some metadata initialized, we also included two bytes worth of data at the very beginning of the array: array[0] == 0xFF and array[1] == 0x55. If both of these values matched, we could reasonably assume that our my_malloc function had been called an initialized already.
 
Workloads:
The run time for workload A is 0.006474 microseconds
The run time for workload B is 0.025296 microseconds
The run time for workload C is 0.291679 microseconds
The run time for workload D is 0.341097 microseconds
The run time for workload E is 0.002967 microseconds
The run time for workload F is 0.134013 microseconds

An interesting finding was the result of workload A compared to workload E. In A we were just malloc()ing 1 byte and then immediately free()ing it but in E we malloc() max memory capacity and also free() it immediately. It turns out that workload E was faster than A. The runtime of workload E is probably smaller because workload A required creating extra metadata and then removing said metadata each iteration, while workload E was mallocing sufficiently large sized chunks that no additional metadata needed to be created and then removed each iteration.
The result that was not surprising was workload D having the longest runtime which could have been caused by the rand() function.
