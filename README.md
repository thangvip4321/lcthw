# lcthw
my exercise on Zed Shaw learn C the hard way
# About the stat_server exe:
- Support string-wise database storage (Ternary Search Tree data structure).
- Maximum 10 clients at the same time.
- URL-wise database storage support ,which mean each URL and its child have their own storage and the parent will own some information of the children, any changes made in the children will also be reflected backward to the parent. (Still, half-baked, only support '/' as the delimiter, also the behaviour between children and parent is not perfect yet).
