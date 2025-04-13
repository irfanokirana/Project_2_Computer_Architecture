1. Describe how your prefetcher works.

The custom prefetcher that I implemented is a simplied version of the Basic Reference Prediction (BRP) staregy from Chen and Baer (1995). THe main idea of this prefetcher to learn from the history of the access pattern of a specific address. To implement this prefetcher, I created a prediction table struct that sotres the last address that was accessed, the stride between the last two accesses, and whether or not that entry is valid (meaning the entry in the prediction table has already been initialized). When a new access occurs, the prefetcher will hash the address to find the corresponding entry in the prediction table. If the entry is valid meaning it has been initialized and there exists a stride, that means there is history that can be used to hopefully improve the prefetcher's prediction. The prefetcher will use the stride to compute the predicted next address and prefetches. If the entry is valid but the stride is zero, no prefetch was made. Either way, if the stride is valid, the stride of that entry in the prediction table is updated. If the entry is not valid, then the validity and stride of that entry is initialized. Regardless of whether the entry was initially valid or not, the entry's last address is updated to be the curent address to setup the prefether for the next time the address is accessed. 

2. Explain how you chose that prefetch strategy.

I first read about the BRP stragetgy from the Chen and Baer paper, and I liked it because it is hardware efficient and does not require too much storage. The stragey seems relatively simple but it has been shown to be effective, particurly for programs that have regular stride based access patterns. Programs like this are common, such as loops or programs that use arrays. I think the stragety is a step up from the sequential prefetcher and utilizes the history of accesses to improve the accuracy of the prefetcher. 

3. Discuss the pros and cons of your prefetch strategy.

Some pros of the custom prefetcher I implemented is that it is lightweight, effective for programs with regular patterns, and requires minimum prefetching overhead. This prefetcher only requires a small amount of storage for each table entry for the last_address, stride, and valid bit making it an extremely lightweight implementation. Additionally, this strategy improves in accuracy compared to the adjacent or sequential prefetcher especially for programs that implement loops, use arrays, or eprform matrix operations since they all use stride based memory access. Finally, this strategy reduces the riskof cache pollution since it only prefetches one line per access. This means that you don't risk prefetching too many lines that may not even be used like the sequential prefetcher. 

However, this prefetcher has its limitations. This simplified BRP implementation is not very effective for programs that show irregular patterns such as programs that use hash tables In this case, the stride of the entry in he prediction table becomes unstable and the prediction is not accurate. Additionally, this implementation has a high risk of causing aliasing in the prediction table. Irregular patterns and the fact that a hash on the address is used to select table entries may lead to unrelated accesses that may overwrite each other's history.

4. Demonstrate that the prefetcher could be implemented in hardware (this can be
   as simple as pointing to an existing hardware prefetcher using the strategy
   or a paper describing a hypothetical hardware prefetcher which implements
   your strategy).

Similar techniques to this prefetcher have already been implemented in hardware. The Chen and Baer paper proposes the implementation of the Basic Reference Prediction Strategy in hardware [1]. The paper explains the hardware table structure that stores the address and stride and performs accesses to this table each time memory is referenced, similar to the implmentation of this prefetcher. Additionally, there are many modern processors that implement stride prefetching similar to the ideas used to my custom prefetcher. These processors include, Intel's prefetchers that is stride based and tracks the distancebetween sequential accesses [2]. ARM and AMD CPU's also implements hardware stride prefetchers that track simple patterns [3]. 

5. Cite any additional sources that you used to develop your prefetcher.

References:
[1] Tien-Fu Chen and Jean-Loup Baer, "Effective hardware-based data prefetching for high-performance processors," in IEEE Transactions on Computers, vol. 44, no. 5, pp. 609-623, May 1995, doi: 10.1109/12.381947.
keywords: {Prefetching;Delay;Hardware;Bandwidth;Counting circuits;Computer science;Timing;Predictive models;Bridges;Coherence}

[2] “Hardware Prefetch Controls for Intel® Atom® Cores.” Intel, www.intel.com/content/www/us/en/content-details/795247/hardware-prefetch-controls-for-intel-atom-cores.html

[3] Documentation – Arm Developer, developer.arm.com/documentation/100048/0100/level-1-memory-system/memory-prefetching/data-prefetching-and-monitoring