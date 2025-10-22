
import srt
from deepseek import Deepseek

def translate_srt_with_deepseek(input_file, output_file, target_language):
    api_key = "YOUR_DEEPSEEK_API_KEY" # Replace with your actual key
    client = Deepseek(api_key=api_key)

    with open(input_file, 'r', encoding='utf-8') as f:
        content = f.read()
        subs = list(srt.parse(content))

    translated_subs = []
    for sub in subs:
        # Use a specific prompt to ensure formatting is maintained
        prompt = (
            f"Translate the following text into {target_language}. "
            "Only respond with the translated text, nothing else.\n\n"
            f"Text: {sub.content}"
        )

        response = client.chat.completions.create(
            model="deepseek-r1", # Or other available DeepSeek models
            messages=[{"role": "user", "content": prompt}]
        )
        translated_text = response.choices[0].message.content

        translated_sub = srt.Subtitle(
            index=sub.index,
            start=sub.start,
            end=sub.end,
            content=translated_text
        )
        translated_subs.append(translated_sub)

    with open(output_file, 'w', encoding='utf-8') as f:
        f.write(srt.compose(translated_subs))

# Example usage
translate_srt_with_deepseek('input.srt', 'translated.srt', 'French')
