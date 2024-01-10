# Lab report 1

* Carlos A. Jarro
* Anna Conda

## This is my lab report
This report includes text and also figures that I linked from my repository.
Like this:
![A test image](https://github.com/cjarro-uky/BAE305-SP24-Lab1/blob/main/20240110_100436.jpg)

Or perhaps a nicer smaller, centered image like this (using html):

<p align="center">
  <img src=https://github.com/cjarro-uky/BAE305-SP24-Lab1/blob/main/20240110_100436.jpg width=50%>
</p>

Also I can include math functions like this:

$$V(V) = I(A)*R(&Omega;)$$

$$P = I^2*R$$

Also I have to include code like this:

```c++
void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    
    while (mySerial.available() > 0 && newData == false) {
        rc = mySerial.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
}
```
Also I can make **bold** and *emphatic* statements and add tables like this:

| Variable | Value |
|:---:|---|
|   V      | 5 V   |
| R        | 1 k&Omega; |
| I        | 5 mA  |

Check out these links:

[Link for images](https://docs.github.com/en/communities/documenting-your-project-with-wikis/editing-wiki-content)

[Link for code](https://docs.github.com/en/get-started/writing-on-github/working-with-advanced-formatting/creating-and-highlighting-code-blocks)

[Link for tables](https://docs.github.com/en/get-started/writing-on-github/working-with-advanced-formatting/organizing-information-with-tables)
