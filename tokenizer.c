
/**
 * count_words - counts the number of words in a string
 * @str: the input string
 * @delimiter: the delimiter character
 *
 * Return: the number of words
 */
int count_words(char *str, char delimiter) {
    int i, num_words = 0;

    if (str == NULL || str[0] == '\0')
        return 0;

    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] != delimiter && (str[i + 1] == delimiter || str[i + 1] == '\0'))
            num_words++;
    }

    return num_words;
}

/**
 * split_string - splits a string into words
 * @str: the input string
 * @delimiter: the delimiter character
 *
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **split_string(char *str, char delimiter) {
    int i, j, k, m, num_words = count_words(str, delimiter);
    char **result;

    if (num_words == 0)
        return NULL;

    result = malloc((1 + num_words) * sizeof(char *));
    if (!result)
        return NULL;

    for (i = 0, j = 0; j < num_words; j++) {
        while (str[i] == delimiter && str[i] != '\0')
            i++;

        k = 0;
        while (str[i + k] != delimiter && str[i + k] != '\0')
            k++;

        result[j] = malloc((k + 1) * sizeof(char));
        if (!result[j]) {
            for (k = 0; k < j; k++)
                free(result[k]);
            free(result);
            return NULL;
        }

        for (m = 0; m < k; m++)
            result[j][m] = str[i++];
        result[j][m] = '\0';
    }

    result[j] = NULL;
    return result;
}
