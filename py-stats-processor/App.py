import pandas as pd
import argparse


def process_data(input_file, output_file):
    df = pd.read_csv(input_file, names=[
                     'src_ip', 'dst_ip', 'src_port', 'dst_port', 'packets', 'bytes'])

    # aggregate data
    src_df = df.groupby('src_ip').agg({'packets': 'sum', 'bytes': 'sum'}).rename(
        columns={'packets': 'sent_packets', 'bytes': 'sent_bytes'})
    dst_df = df.groupby('dst_ip').agg({'packets': 'sum', 'bytes': 'sum'}).rename(
        columns={'packets': 'received_packets', 'bytes': 'received_bytes'})

    result_df = pd.concat([src_df, dst_df], axis=1).fillna(0)
    result_df.to_csv(output_file, index_label='ip_address')


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Process pcap stats data')
    parser.add_argument('-i', '--input', help='Input CSV file', required=True)
    parser.add_argument(
        '-o', '--output', help='Output CSV file', required=True)

    args = parser.parse_args()
    process_data(args.input, args.output)
