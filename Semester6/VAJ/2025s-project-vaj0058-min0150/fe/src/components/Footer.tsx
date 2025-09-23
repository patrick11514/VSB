import Icon from './Icon';

export default function Footer() {
    return (
        <footer className="border-t-text flex items-center justify-center border-t-2 py-0.5">
            StarTech &copy; {new Date().getFullYear()} | Created with&nbsp;
            <Icon name="bi-heart-fill" className="text-red-500" />
            &nbsp;by&nbsp;
            <a className="underline" href="https://github.com/OndraVajda" target="_blank">
                Ondřej Vajda
            </a>
            &nbsp;&&nbsp;
            <a className="underline" href="https://patrick115.eu" target="_blank">
                Patrik Mintěl
            </a>
        </footer>
    );
}
