import sys
import subprocess

def save_file(partition, lang, sentences):
    """
    saves the sentences to a file.
    """
    with open('data/{}/{}.train'.format(partition, lang), 'w') as model_file:
        model_file.writelines(sentence + "\n" for sentence in sentences)



def load_tagged_file(filename):
    """
    Reads the part-of-speech-tagged data in "word[\t]tag[\n]" format
    as a list of sentences, which are in turn lists of <word, tag> pairs.
    """
    sentences = []
    with open(filename) as file:
        sentence = ""
        for line in file:
            line = line.strip()
            if line:
                sentence += line + "\n"
            else:  # empty line is sentence delimiter
                sentences.append(sentence)
                sentence = ""

    return sentences

def main():
    """
    Main loops
    """
    partitions = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100]
    langs = ["es", "et", "fi", "nl", "no"]

    for lang in langs:
        sentences = load_tagged_file("data/{}.train".format(lang))
        total = len(sentences)
        for partition in partitions:
            save_file(partition, lang, sentences[:int(total*(partition/float(100)))])
            subprocess.call(
                ['tnt-para -o models/{}/{}'.format(partition, lang) +
                 ' data/{}/{}.train'.format(partition, lang)], shell=True)
            subprocess.call(
                ['tnt models/{}/{} '.format(partition, lang) +
                 'data/{}.test '.format(lang) +
                 '> data/{}/{}.test.tagged'.format(partition, lang)], shell=True)


if __name__ == '__main__':
    main()
