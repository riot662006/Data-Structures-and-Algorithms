import java.util.*;

/*
 * Algorithm that checks whether str2 is a substring of str1
 */
class StringMatching {
    public static boolean bruteForce(String str1, String str2) {
        // Time complexity - O(n^2), Space complexity - O(1)
        // if str2 is larger than str1, it can not be a subset
        if (str1.length() < str2.length()) {
            return false;
        }

        for (int i = 0; i < str1.length() - str2.length() + 1; ++i) { // position of first letter of str2 on str1
            for (int j = 0; j < str2.length(); ++j) { // position of current checked character on str2
                if (str1.charAt(i + j) == str2.charAt(j)) {
                    // if reached the end of the second string, the string matches
                    if (j == str2.length() - 1) return true;
                    continue;
                }
                // if the current character does not match, the strings do not match
                break;
            }
        }

        return false;
    }

    public static void main(String[] args) {
        String str1 = "Hello world";
        String str2 = "world";

        System.out.println(bruteForce(str1, str2));
    }
};