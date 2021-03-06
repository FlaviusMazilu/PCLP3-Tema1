Copyright 2022 Maria Sfiraiala Flavius Mazilu (314CA)

# Desperate moments - Project 1

-------------------------------------------------------------------------------

## Description:
- The project aims to linearize multiple strcutures, by copying each one of
them into a generic array
- Every structure, prior to its transfer into the polymorphic array, gets a
header which contains metadata about its content
- The header is copied into the array, and immediately after that we'll copy
the structure
- Every structure contains, in this order, a name (a string of variable
length), two bills (ints of different sizes, got from the inttypes.h header)
and another name (of variable length)
- The supported opperations are add_last, add_at, delete_at, find and print

-------------------------------------------------------------------------------

## ADD_LAST
- Its driver function reads the information that we'll use for the structure:
except for the header's type, everything else will be read as a string
- Then, we get rid off the extra space we used for reading the strings
- The int values are going to be transformed from strings to integers using
atoi()
- However, every single one of these transformations is going to be perfomed
according to the type of header
- We'll also update structure's metadata: its length will be represented by
the sum of sizes for the ints plus the sum of lengths for the strings
plus an extra 2 for every '\0'
- The last step, right before merging the information into the big array, is to
reallocate the array's memory

- The main add_last function gets the total number of bytes contained by the
array and at that position copies the header and the data

-------------------------------------------------------------------------------

## ADD_AT
- Its driver function is built just like the driver function for the add_last
operation, the only difference is the fact that we'll also read an index, which
points at the position we want the data package to be inserted at

- The main add_at function is based on determining both the total number of
bytes and the number of bytes until the data package that resides at the index
position
- We, then, define a separate array which will contain the last part of the array
(the one that needs to be shifted in order to make space for the new structure)
- We insert the desired data package, and we bring the last part of the array
back

-------------------------------------------------------------------------------

## FIND
- Its driver function only reads the index for the package we want to print and
validates it

- The first step in finding the data package is to calculate the number of
bytes until its position
- We define a pointer which indicates the memory block at which the data
package starts
- We can safely print the header's type and after this, we move the pointer
sizeof(uchar) positions at the right
- Printing the pointer after casting it to a pointer of char gets the first
name, since printf() only prints strings until it reaches '\0'
- We shift the pointer at the right so now it points to the first integer
- Again, depending on the header's type, we print each integer and move the
pointer further to the right
- Similarly with the first string, we print the second name

-------------------------------------------------------------------------------

## DELETE_AT
- Its driver function determines the index we'll be deleting the data package
from
- It also recalculates the total number of bytes contained by the array,
considering a future deletion and reallocates the space

- The main delete_at function overwrites the data package that we want to
remove: firstly, we get the number of bytes until the index, then the number of
bytes until the next data package after the index and, lastly, the total number
of bytes of the array
- We ignore what resides between the index and index + 1 and we copy the data
that starts at index + 1 over what starts at index

-------------------------------------------------------------------------------

## PRINT
- We interate through the polymorphic array and for every package we follow the
same pattern we used in the FIND function

-------------------------------------------------------------------------------

## MAIN
- We read what's given from stdin line by line, using fgets
- We stop when we reach the exit command
- From the line that was read earlier we should determine the operation's
keyword
- After doing so, what remains in the input should be everything else beside
the operation
- We'll also be careful about memory allocation errors, which will be
solved accordingly in each function; mem_ups represents a flag for such
situations

-------------------------------------------------------------------------------